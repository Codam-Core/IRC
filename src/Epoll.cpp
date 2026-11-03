#include "Epoll.hpp"

// Constructors
Epoll::Epoll(void) : _epoll_fd(epoll_create1(0))
{
	if (_epoll_fd.get_fd() == -1)
		throw std::runtime_error(std::string(FAILED_TO_CREATE_EPOLL_MSG) + std::strerror(errno));

	std::cerr << colour::GREEN << "[Epoll] " << colour::RESET << "constructor called: epoll fd " << _epoll_fd.get_fd() << std::endl;
}

// Destructor
Epoll::~Epoll(void)
{
	std::cerr << colour::GREEN << "[Epoll] " << colour::RESET << "destructor called" << std::endl;
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

	std::cerr << colour::GREEN << "[Epoll] " << colour::RESET << "added fd " << fd << std::endl;
}

void Epoll::remove(int fd)
{
	epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_DEL, fd, nullptr);
	std::cerr << colour::GREEN << "[Epoll] " << colour::RESET << "removed fd " << fd << std::endl;
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
