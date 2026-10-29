#include "ListeningSocket.hpp"

// Constructors
ListeningSocket::ListeningSocket(uint16_t port) : _ls_fd(socket(AF_INET, SOCK_STREAM, 0)) // create socket, uses IPv4 and TCP
{
	if (_ls_fd.get_fd() == -1)
		throw std::runtime_error(std::string(FAILED_TO_CREATE_SOCKET_MSG) + std::strerror(errno));

	int optval = 1;
	if (setsockopt(_ls_fd.get_fd(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) // turn on SO_REUSEADDR so when server stops we can restart right away (instead of waiting 1 minute)
		throw std::runtime_error(std::string(FAILED_TO_SET_SOCKET_OPTION_MSG) + std::strerror(errno));

	if (fcntl(_ls_fd.get_fd(), F_SETFL, O_NONBLOCK) == -1) // make socket non-blocking
		throw std::runtime_error(std::string(FAILED_TO_SET_NONBLOCKING_MSG) + std::strerror(errno));

	sockaddr_in server_addr = {};				// write the address (and set all bytes to 0 (-> {}) :
	server_addr.sin_family = AF_INET;			// IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;	// listen to ALL network interfaces
	server_addr.sin_port = htons(port);			// Host TO Network Short -> makes sure bytes are stored in the right order (big end first, instead of little end first (like the computer stores it))

	if (bind(_ls_fd.get_fd(), reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1) // put the adress in the socket
		throw std::runtime_error(std::string(FAILED_TO_BIND_SOCKET_MSG) + std::to_string(port) + ": " + std::strerror(errno));

	if (listen(_ls_fd.get_fd(), SOMAXCONN) == -1) // start accepting connections
		throw std::runtime_error(std::string(FAILED_TO_LISTEN_MSG) + std::strerror(errno));

	std::cerr << colour::YELLOW << "[ListeningSocket] " << colour::RESET << "constructor called: listening on 0.0.0.0:" << port << " with fd " << _ls_fd.get_fd() << std::endl;
}

// Destructor
ListeningSocket::~ListeningSocket(void)
{
	std::cerr << colour::YELLOW << "[ListeningSocket] " << colour::RESET << "destructor called" << std::endl;
}

// Getters and Setters
int ListeningSocket::get_fd() const
{
	return (_ls_fd.get_fd());
}

// Member functions
Fd ListeningSocket::accept_client()
{
	int client_fd = accept(_ls_fd.get_fd(), nullptr, nullptr);
	if (client_fd == -1) // read errno right away, before anything else can change it
		throw std::runtime_error(std::string(FAILED_TO_ACCEPT_MSG) + std::strerror(errno));

	return (Fd(client_fd)); // only a valid fd gets wrapped in an Fd
}
