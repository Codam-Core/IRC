#include "ConnectedSocket.hpp"

// Constructors
ConnectedSocket::ConnectedSocket(Fd fd) : _cs_fd(std::move(fd))
{
	if (fcntl(_cs_fd.get_fd(), F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(std::string(FAILED_TO_SET_NONBLOCKING_MSG) + std::strerror(errno));

	std::cerr << colour::CYAN << "[ConnectedSocket] " << colour::RESET << "constructor called: client fd " << _cs_fd.get_fd() << " is non-blocking" << std::endl;
}

// Destructor
ConnectedSocket::~ConnectedSocket(void)
{
	std::cerr << colour::CYAN << "[ConnectedSocket] " << colour::RESET << "destructor called; client fd was " << _cs_fd.get_fd() << std::endl;
}

// Getters and Setters
int ConnectedSocket::get_fd() const
{
	return (_cs_fd.get_fd());
}

// Member functions
ssize_t ConnectedSocket::recv_data(char *buffer, size_t size)
{
	return (recv(_cs_fd.get_fd(), buffer, size, 0));
}

