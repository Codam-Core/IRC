#include "Server.hpp"

// Constructor
Server::Server(const std::string &port, const std::string &password) : _port(parse_port(port)), _password(parse_password(password)), _listeningSocket(_port)
{
	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "constructor called: port " << _port << ", password: " << _password << std::endl;

	_epoll.add(_listeningSocket.get_fd());

	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "added listening fd " << _listeningSocket.get_fd() <<  " to epoll fd " << _epoll.get_fd() << "." << std::endl;
}

// Destructor
Server::~Server(void)
{
	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "destructor called" << std::endl;
}

// Getters and Setters
uint16_t Server::get_port() const
{
	return (_port);
}

std::string Server::get_password() const
{
	return (_password);
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
	int event_count = _epoll.wait(event_array, MAX_EVENTS);

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
				ConnectedSocket client(_listeningSocket.accept_client());
				int client_fd = client.get_fd();
				
				_epoll.add(client_fd);
		
				_clients.emplace(client_fd, std::move(client));
				std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "accepted new client with fd " << client_fd << std::endl;
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
			auto it = _clients.find(fd);
			if (it == _clients.end())
				continue;
			if (events & EPOLLERR)
			{
				disconnect_client(fd);
				continue;
			}
			if (events & EPOLLIN)
			{
				char buffer[512];
				ssize_t bytes = it->second.recv_data(buffer, sizeof(buffer));
				if (bytes <= 0)
					disconnect_client(fd);
				else
					std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "fd " << fd << " says: " << std::string(buffer, bytes);
			}
			else if (events & (EPOLLHUP | EPOLLRDHUP))
				disconnect_client(fd);
		}
	}
}

void Server::disconnect_client(int fd)
{
	_epoll.remove(fd);
	_clients.erase(fd);
	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "client fd " << fd << " disconnected" << std::endl;
}

