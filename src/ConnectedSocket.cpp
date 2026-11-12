#include "ConnectedSocket.hpp"
#include "debug.hpp"

// Constructors
ConnectedSocket::ConnectedSocket(Fd fd) : _cs_fd(std::move(fd))
{
	if (fcntl(_cs_fd.get_fd(), F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(std::string(FAILED_TO_SET_NONBLOCKING_MSG) + std::strerror(errno));

	debug(colour::CYAN, "ConnectedSocket", "constructor called: client fd " + std::to_string(_cs_fd.get_fd()) + " is non-blocking");
}

// Destructor
ConnectedSocket::~ConnectedSocket(void)
{
	debug(colour::CYAN, "ConnectedSocket", "destructor called; client fd was " + std::to_string(_cs_fd.get_fd()));
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
	{
		// no complete line yet. if it's already too long, keep only the start so the buffer can't grow forever;
		// when the \n arrives, the line is still too long and the server rejects it
		if (_recv_buffer.size() > MAX_LINE_LENGTH + 1)
			_recv_buffer.resize(MAX_LINE_LENGTH + 1);
		return (false);
	}

	line = _recv_buffer.substr(0, end);
	if (!line.empty() && line.back() == '\r')
		line.pop_back();

	_recv_buffer.erase(0, end + 1);
	return (true);
}

void ConnectedSocket::queue_send(const std::string &message)
{
	// std::cout << "Message queued for fd " << _cs_fd.get_fd() << ": " << message << std::endl;
	_send_buffer.append(message);
}

ssize_t ConnectedSocket::send_data()
{
	// debug:  we're about to send, without the trailing \r\n (debug() adds its own newline)
	std::string pending = _send_buffer;
	while (!pending.empty() && (pending.back() == '\n' || pending.back() == '\r'))
		pending.pop_back();
	// debug(colour::GREEN, "Send", "fd " + std::to_string(_cs_fd.get_fd()) + ": " + pending);

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
