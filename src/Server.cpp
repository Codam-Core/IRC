#include "Server.hpp"
#include "debug.hpp"

// Constructor
Server::Server(const std::string &port, const std::string &password) : _port(parse_port(port)), _password(parse_password(password)), _listeningSocket(_port), _commands(*this)
{
	debug(colour::MAGENTA, "Server", "constructor called: port " + std::to_string(_port) + ", password: " + _password);

	_epoll.add(_listeningSocket.get_fd());

	debug(colour::MAGENTA, "Server", "added listening fd " + std::to_string(_listeningSocket.get_fd()) + " to epoll fd " + std::to_string(_epoll.get_fd()));
}

// Destructor
Server::~Server(void)
{
	debug(colour::MAGENTA, "Server", "destructor called");
}

// Getters and Setters
uint16_t Server::get_port() const
{
	return (_port);
}

bool Server::check_password(Client& client, const std::string &password)
{
	if (_password == password)
	{
		client.set_password(password);
		return (true);
	}
	else
	{
		client.mark_for_close();
		return (false);
	}
}

// Private member functions
std::string Server::parse_password(const std::string &password_str)
{
	if (password_str == "")
	{
		throw std::invalid_argument(INVALID_PASSWORD_MSG);
	}
	else
	{
		return (password_str);
	}
}

uint16_t Server::parse_port(const std::string &port_str)
{
	if (port_str.length() == 0 || port_str.length() > 5)
	{
		throw std::invalid_argument(INVALID_PORT_MSG);
	}

	for (size_t i = 0; i < port_str.length(); i++)
	{
		if (!isdigit(static_cast<unsigned char>(port_str[i])))
		{
			throw std::invalid_argument(INVALID_PORT_MSG);
		}
	}

	int port_nbr = std::stoi(port_str);
	if (port_nbr < 1 || port_nbr > 65535)
	{
		throw std::invalid_argument(INVALID_PORT_MSG);
	}
	else 
	{
		return (static_cast<uint16_t>(port_nbr));
	}
}

// Public member functions
void Server::update()
{
	epoll_event event_array[MAX_EVENTS];
	int event_count = _epoll.wait(event_array, MAX_EVENTS); // waits till something is happening

	if (event_count == -1)
	{
		if (g_stop == 1)  // no error: we were woken up to stop
			return ;
		else // real error
			throw std::runtime_error(std::string(FAILED_TO_WAIT_FOR_EPOLL_MSG) + std::strerror(errno));
	}

	for (int i = 0; i < event_count; i++)
	{
		if (event_array[i].data.fd == _listeningSocket.get_fd()) // the listening socket is ready → a NEW client is knocking: accept it, keep it, and add it to epoll
		{
			try
			{
				ConnectedSocket socket(_listeningSocket.accept_client());
				int client_fd = socket.get_fd();
				
				_epoll.add(client_fd);
		
				_clients.emplace(client_fd, std::move(socket));
				debug(colour::MAGENTA, "Server", "accepted new client with fd " + std::to_string(client_fd));
			}
			catch (const std::exception &e)
			{
				std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << e.what() << std::endl;
			}
		}
		else // any other fd is an EXISTING client that is ready → it sent us data (or it hung up): read it
		{
			int fd = event_array[i].data.fd;
			uint32_t events = event_array[i].events;
			auto client_it = _clients.find(fd);
			if (client_it == _clients.end())
				continue;
			Client &client = client_it->second;

			if (events & EPOLLERR)
			{
				disconnect_client(client);
				continue;
			}
			if (events & EPOLLIN)
			{
				if (!read_from_client(client)) // client disconnected so we must skipp the EPOLLOUT part
					continue; 
			}
			else if (events & (EPOLLHUP | EPOLLRDHUP))
			{
				disconnect_client(client);
				continue;
			}
			if (events & EPOLLOUT)
			{
				if (client.get_socket().send_data() < 0)
				{
					disconnect_client(client);
					continue;
				}
				if (!client.get_socket().has_pending_send())
				{
					if (client.is_marked_for_close())
					{
						disconnect_client(client);
						continue;
					}
					else
						_epoll.modify(fd, EPOLLIN | EPOLLRDHUP);
				}
			}
		}
	}
}

void Server::disconnect_client(Client &client)
{
	// tell the others (if they didn't QUIT already) and leave all channels
	leave_all_channels(client, "Connection closed");


	int fd = client.get_fd();
	_epoll.remove(fd);
	_clients.erase(fd);
	debug(colour::MAGENTA, "Server", "client fd " + std::to_string(fd) + " disconnected");
}

bool Server::read_from_client(Client &client) // return false? -> client disconnected
{
	// read data from the client socket into the receive buffer
	ssize_t bytes = client.get_socket().recv_data();
	std::string line;
	
	// if we received 0 bytes, the client has closed the connection, so we disconnect it
	if (bytes <= 0)
	{
		disconnect_client(client);
		return (false);
	}

	// if the client is marked for close, we don't want to process any more data from it
	if (client.is_marked_for_close())
		return (true);
	
	// fetch the line from the receiving buffer. only complete lines will be proceesed (\n).
	while (client.get_socket().get_line(line))
	{		
		// a line longer than the IRC limit is not handled
		if (line.size() > ConnectedSocket::MAX_LINE_LENGTH)
		{
			CommandStream error_stream("", *this);
			error_stream.prefix().code(417).client(client).message("Input line was too long").finish();
			error_stream.send(client);
			continue;
		}

		// parse the command and its arguments from the line
		parse_command(client, line);
		
		// a command may have marked the client for close: stop handling its other lines
		if (client.is_marked_for_close())
			break;
	}
	return (true);
}

void Server::parse_command(Client &client, const std::string &line)
{
	// convert the line into a stringstream to easily parse the command and its arguments
	std::stringstream ss(line);
	std::string cmd;
	std::vector<std::string> arguments;

	// parse the command and its arguments from the line
	if (!(ss >> cmd))          // empty line or only spaces → nothing to do
		return;

	if (cmd[0] == ':')         // a prefix → skip it; the command is the next word
	{
		if (!(ss >> cmd))      // only a prefix, no command
			return;
	}
	
	cmd = to_upper(cmd);

	std::string argument;

	// extract all words untill nothing is left.
	while (ss >> argument)
	{
		// handle a special case where the last argument start with a colon (:).
		// in this case, we want to treat the rest of the line as a single argument, including spaces.
		if (!argument.empty() && argument[0] == ':')
		{
			std::string rest;

			// getline reads from the rest of the stringstream until EOF is found, and stores it in the rest variable.
			std::getline(ss, rest);

			// remove the (":") from the argument, and add the rest of the line to it, and push it to the arguments vector.
			arguments.push_back(argument.substr(1) + rest);
			break;
		}

		// add the argument to the list of arguments
		arguments.push_back(argument);
	}

	// handle the command with its arguments
	_commands.handle_command(client, cmd, arguments);
}

void Server::send_to_client(Client &client, const std::string &message)
{
	client.get_socket().queue_send(message);
	_epoll.modify(client.get_fd(), EPOLLIN | EPOLLRDHUP | EPOLLOUT); 
}

Channel *Server::find_channel(const std::string &name)
{
	auto it = _channels.find(to_lower(name));
	if (it == _channels.end())
		return (nullptr);
	return (&it->second);
}

const std::map<std::string, Channel> &Server::get_channels() const
{
	return (_channels);
}

Channel &Server::get_or_create_channel(const std::string &name)
{
	std::string channel = to_lower(name);
	auto result = _channels.try_emplace(channel, channel); // if channel isn't a key yet, it builds Channel(channel) directly inside the map. If the key already exists, it does nothing.
	return (result.first->second);
}

bool Server::nickname_taken(const std::string &nickname) const
{
	for (auto& [fd, client] : _clients)
	{
		if (to_lower(client.get_nickname()) == to_lower(nickname))
			return (true);
	}
	return (false);
}

Client* Server::fetch_client_by_nickname(const std::string &nickname)
{
	for (auto& [fd, other] : _clients)
	{
		if (to_lower(other.get_nickname()) == to_lower(nickname))
		{
			return (&other);
		}
	}
	return (nullptr);
}

void Server::remove_client_from_channel(Client &client, Channel& channel)
{
	// remove the client from members and invites; 
	channel.remove_member(&client);
	channel.remove_invite(&client);
	debug(colour::MAGENTA, "Server", "removed client with fd " + std::to_string(client.get_fd()) + " from " + channel.get_name() + ", members left: " + std::to_string(channel.member_count()));
	// erase the channel if it's empty now
	if (channel.member_count() == 0)
		_channels.erase(channel.get_name());
}

void Server::remove_client_from_all_channels(Client &client)
{
	auto it = _channels.begin();
	while (it != _channels.end())
	{
		auto current = it;
		++it;
		remove_client_from_channel(client, current->second);
	}
}

void Server::leave_all_channels(Client &client, const std::string &reason)
{
	// everyone who shares a channel with the client
	std::set<Client *> receivers = get_clients_sharing_a_channel(client);

	// tell them all that the client quit
	CommandStream quit_stream("QUIT", *this);
	quit_stream.prefix(client).command().message(reason).finish();
	for (Client *receiver : receivers)
		quit_stream.send(*receiver);

	// now remove the client from every channel (and erase channels that became empty)
	remove_client_from_all_channels(client);
}

// everyone who is in at least one channel together with the client (not the client itself), each only once
std::set<Client *> Server::get_clients_sharing_a_channel(Client &client)
{
	// collect everyone who shares a channel with the client; a set keeps each one only once
	std::set<Client *> others;
	for (auto &[name, channel] : _channels)
	{
		(void)name;
		if (!channel.is_member(&client))
			continue;
		for (auto &[member, is_operator] : channel.get_members())
		{
			(void)is_operator;
			if (member != &client)
				others.insert(member);
		}
	}
	return (others);
}
