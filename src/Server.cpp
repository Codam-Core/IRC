#include "Server.hpp"

// Constructor
Server::Server(const std::string &port, const std::string &password) : _port(parse_port(port)), _password(parse_password(password)), _listeningSocket(_port), _epoll_fd(epoll_create1(0))
{
	
	if (_epoll_fd.get_fd() == -1)
		throw std::runtime_error(std::string(FAILED_TO_CREATE_EPOLL_MSG) + std::strerror(errno));
	
	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "constructor called: port " << _port << ", password: " << _password << std::endl;

	epoll_event listen_event = {};
	listen_event.events = EPOLLIN; // tells the server (?) that a new client is waiting, there is something new to read!
	listen_event.data.fd = _listeningSocket.get_fd();
	
	if (epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_ADD, listen_event.data.fd, &listen_event) == -1)
		throw std::runtime_error(std::string(FAILED_TO_ADD_CLIENT_TO_EPOLL_MSG) + std::strerror(errno));

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
void Server::run()
{
	epoll_event listen_event_array[MAX_EVENTS];

	while (true)
	{
		int events = epoll_wait(_epoll_fd.get_fd(), listen_event_array, MAX_EVENTS, -1);
		if (events == -1)
			throw std::runtime_error(std::string(FAILED_EPOLL_WAIT_MSG) + std::strerror(errno));

		for (int i = 0; i < events; i++)
		{
			if (listen_event_array[i].data.fd == _listeningSocket.get_fd())
				std::cout << colour::MAGENTA << "[Server] " << colour::RESET << " new client is waiting" << std::endl;
		
		}
	}

}