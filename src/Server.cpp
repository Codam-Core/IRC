#include "Server.hpp"
#include "colours.hpp"


// Constructor
Server::Server(const std::string &port, const std::string &password) : _port(parse_port(port)), _password(parse_password(password)), _socket_fd(-1)
{
	std::cout << colour::BLUE << "Server parameterized constructor is called" << colour::RESET << std::endl;
}

// Destructor
Server::~Server(void)
{
	if (_socket_fd != -1)
		close(_socket_fd);

	std::cout << colour::BLUE << "Server destructor is called" << colour::RESET << std::endl;
}

// Getters and Setters
size_t Server::get_port() const
{
	return (_port);
}

std::string Server::get_password() const
{
	return (_password);
}

int Server::get_socket_fd() const
{
	return (_socket_fd);
}

// Member functions
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

size_t Server::parse_port(const std::string &port_str)
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
		return ((size_t)port_nbr);
	}
}

void Server::create_socket()
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // uses IPv4 and TCP
	if (_socket_fd == -1)
		throw std::runtime_error(std::string(FAILED_TO_CREATE_SOCKET_MSG) + std::strerror(errno));

	int optval = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		throw std::runtime_error(std::string(FAILED_TO_SET_SOCKET_OPTION_MSG) + std::strerror(errno));

	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(std::string(FAILED_TO_SET_NONBLOCKING_MSG) + std::strerror(errno));

	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
  	sin.sin_addr.s_addr = INADDR_ANY;
  	sin.sin_port = htons(_port);

	if (bind(_socket_fd, reinterpret_cast<sockaddr *>(&sin.sin_addr.s_addr), sizeof(sin)) == -1)
		throw std::runtime_error(std::string(FAILED_TO_BIND_SOCKET_MSG) + std::to_string(_port) + ": " + std::strerror(errno));



}

// Operators, 
std::ostream& operator<<(std::ostream& output_stream, const Server& src)
{
	output_stream << "parsed port = " << src.get_port() << std::endl;
	output_stream << "parsed password = " << src.get_password() << std::endl;
	output_stream << "socket fd = " << src.get_socket_fd() << std::endl;

	return (output_stream);
}
