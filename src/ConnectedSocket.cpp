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
ssize_t ConnectedSocket::recv_data()
{
	char buffer[RECV_SIZE];
	ssize_t bytes = recv(_cs_fd.get_fd(), buffer, sizeof(buffer), 0);
	if (bytes > 0)
		_recv_buffer.append(buffer, bytes);
	return (bytes);
}

bool ConnectedSocket::get_line(std::string &line)
{
	size_t end = _recv_buffer.find('\n');
	if (end == std::string::npos)
		return (false);

	line = _recv_buffer.substr(0, end);
	if (!line.empty() && line.back() == '\r')
		line.pop_back();

	_recv_buffer.erase(0, end + 1);
	return (true);
}

void ConnectedSocket::queue_send(const std::string &message)
{
	_send_buffer.append(message);
}

ssize_t ConnectedSocket::send_data()
{
	ssize_t bytes = send(_cs_fd.get_fd(), _send_buffer.data(), _send_buffer.size(), MSG_NOSIGNAL);
	if (bytes > 0)
		_send_buffer.erase(0, bytes);
	
	return (bytes);
}

bool ConnectedSocket::has_pending_send() const
{
	if (_send_buffer.empty())
		return (false);
	else
		return (true);
}
