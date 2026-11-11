#include "Epoll.hpp"
#include "debug.hpp"

// Constructors
Epoll::Epoll(void) : _epoll_fd(epoll_create1(0))
{
	if (_epoll_fd.get_fd() == -1)
		throw std::runtime_error(std::string(FAILED_TO_CREATE_EPOLL_MSG) + std::strerror(errno));

	debug(colour::GREEN, "Epoll", "constructor called: epoll fd " + std::to_string(_epoll_fd.get_fd()));
}

// Destructor
Epoll::~Epoll(void)
{
	debug(colour::GREEN, "Epoll", "destructor called");
}

// Getters and Setters
int Epoll::get_fd() const
{
	return (_epoll_fd.get_fd());
}

// Member functions
void Epoll::add(int fd)
{
	epoll_event event = {};
	event.events = EPOLLIN | EPOLLRDHUP;
	event.data.fd = fd;

	if (epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_ADD, fd, &event) == -1)
		throw std::runtime_error(std::string(FAILED_TO_ADD_FD_TO_EPOLL_MSG) + std::strerror(errno));

	debug(colour::GREEN, "Epoll", "added fd " + std::to_string(fd));
}

void Epoll::remove(int fd)
{
	epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_DEL, fd, nullptr);
	debug(colour::GREEN, "Epoll", "removed fd " + std::to_string(fd));
}

int Epoll::wait(epoll_event *events, int max_events)
{
	return (epoll_wait(_epoll_fd.get_fd(), events, max_events, -1));
}

void Epoll::modify(int fd, uint32_t events)
{
	epoll_event event = {};
	event.events = events;
	event.data.fd = fd;
	epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_MOD, fd, &event);
}
