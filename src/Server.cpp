#include "Server.hpp"

// Constructor
Server::Server(const std::string &port, const std::string &password) : _port(parse_port(port)), _password(parse_password(password)), _listeningSocket(_port), _epoll_fd(epoll_create1(0))
{
	
	if (_epoll_fd.get_fd() == -1)
		throw std::runtime_error(std::string(FAILED_TO_CREATE_EPOLL_MSG) + std::strerror(errno));
	
	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "constructor called: port " << _port << ", password: " << _password << std::endl;

	epoll_event listen_event = {};
	listen_event.events = EPOLLIN; // tells the server that a new client is waiting, there is something new to read!
	listen_event.data.fd = _listeningSocket.get_fd();
	
	if (epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_ADD, listen_event.data.fd, &listen_event) == -1)
		throw std::runtime_error(std::string(FAILED_TO_ADD_FD_TO_EPOLL_MSG) + std::strerror(errno));

	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "added listening fd " << _listeningSocket.get_fd() <<  " to epoll fd " << _epoll_fd.get_fd() << "." << std::endl;

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
	int event_count = epoll_wait(_epoll_fd.get_fd(), event_array, MAX_EVENTS, -1);
	if (event_count == -1)
		throw std::runtime_error(std::string(FAILED_TO_WAIT_FOR_EPOLL_MSG) + std::strerror(errno));

	for (int i = 0; i < event_count; i++)
	{
		if (event_array[i].data.fd == _listeningSocket.get_fd()) // the listening socket is ready → a NEW client is knocking: accept it, keep it, and add it to epoll
		{
			try
			{
				ConnectedSocket client(_listeningSocket.accept_client());
				int client_fd = client.get_fd();

				epoll_event client_event = {};
				client_event.events = EPOLLIN;
				client_event.data.fd = client_fd;
				if (epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_ADD, client_fd, &client_event) == -1)
					throw std::runtime_error(std::string(FAILED_TO_ADD_FD_TO_EPOLL_MSG) + std::strerror(errno));

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
			auto it = _clients.find(fd);
			if (it == _clients.end())
				continue;

			char buffer[512];
			ssize_t bytes = it->second.recv_data(buffer, sizeof(buffer));
			if (bytes <= 0)
			{
				epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_DEL, fd, nullptr);
				_clients.erase(it);
				std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "client fd " << fd << " disconnected" << std::endl;
			}
			else
				std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "fd " << fd << " says: " << std::string(buffer, bytes);
		}

	}
}
