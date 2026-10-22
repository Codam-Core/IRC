#include "Server.hpp"
#include "colours.hpp"

// Constructor
Server::Server(const std::string &port, const std::string &password) : _port(parse_port(port)), _password(parse_password(password)), _socket_fd(-1)
{
	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "constructor called: port " << _port << ", password: " << _password << std::endl;
}

// Destructor
Server::~Server(void)
{
	if (_socket_fd != -1)
	{
		std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "destructor called: listening fd " << _socket_fd << " is closed" << std::endl;
		close(_socket_fd);
	}
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

void Server::create_socket() // with comments because this shit is hard to remember
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // create socket, uses IPv4 and TCP
	if (_socket_fd == -1)
		throw std::runtime_error(std::string(FAILED_TO_CREATE_SOCKET_MSG) + std::strerror(errno));

	int optval = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) // turn on SO_REUSEADDR so when server stops we can restart right away (instead of waiting 1 minute)
		throw std::runtime_error(std::string(FAILED_TO_SET_SOCKET_OPTION_MSG) + std::strerror(errno));

	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1) // make socket non-blocking
		throw std::runtime_error(std::string(FAILED_TO_SET_NONBLOCKING_MSG) + std::strerror(errno));

	sockaddr_in server_addr = {}; 				// write the adress (and set all bytes to 0 (-> {}) :
	server_addr.sin_family = AF_INET;			// IPv4
  	server_addr.sin_addr.s_addr = INADDR_ANY; 	// listen to ALL networn interfaces
  	server_addr.sin_port = htons(_port);		// Host TO Network Short -> makes sure bytes are stored in the right order (big end first, insteat of little end first (like the computer stores it))

	if (bind(_socket_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1) // put the adress in the socket
		throw std::runtime_error(std::string(FAILED_TO_BIND_SOCKET_MSG) + std::to_string(_port) + ": " + std::strerror(errno));

	if (listen(_socket_fd, SOMAXCONN) == -1) // start accepting connections
		throw std::runtime_error(std::string(FAILED_TO_LISTEN_MSG) + std::strerror(errno));

	std::cerr << colour::MAGENTA << "[Server] " << colour::RESET << "create_socket called: listening on 0.0.0.0:" << _port << " with fd " << _socket_fd << std::endl;
}
