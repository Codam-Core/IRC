#include "Fd.hpp"

// Constructors
Fd::Fd(int fd) : _fd(fd)
{
	std::cerr << colour::BLUE << "[Fd] " << colour::RESET << "constructor called: now owns fd " << _fd << std::endl;
}

// Move constructor
Fd::Fd(Fd&& other) noexcept : _fd(other._fd)
{
	other._fd = -1; // the old box is now empty, so its destructor won't close our fd

	std::cerr << colour::BLUE << "[Fd] " << colour::RESET << "move constructor called: new object now holds fd " << _fd << std::endl;
}

// Destructor
Fd::~Fd(void)
{
	if (_fd != -1)
	{
		std::cerr << colour::BLUE << "[Fd] " << colour::RESET << "destructor called: fd " << _fd << " is closed" << std::endl;
		close(_fd);
	}
}

// Getters and Setters
int Fd::get_fd() const
{
	return (_fd);
}

// Overload operators
Fd& Fd::operator=(Fd&& other) noexcept
{
	if (this != &other) // self-check: a = std::move(a) must do nothing
	{
		int old_fd = this->_fd; // save it, because we overwrite _fd below

		if (this->_fd != -1)
			close(this->_fd);

		this->_fd = other._fd;
		other._fd = -1;

		std::cerr << colour::BLUE << "[Fd] " << colour::RESET << "move assignment called: fd " << old_fd << " is now closed and holds fd " << this->_fd << std::endl;

	}
	return (*this);
}
