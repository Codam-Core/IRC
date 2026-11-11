#include "Fd.hpp"
#include "debug.hpp"

// Constructors
// Fd::Fd(void) : _fd(-1)
// {
// }

Fd::Fd(int fd) : _fd(fd)
{
	debug(colour::BLUE, "Fd", "constructor called: now owns fd " + std::to_string(_fd));
}

// Move constructor
Fd::Fd(Fd&& other) noexcept : _fd(other._fd)
{
	other._fd = -1; // the old box is now empty, so its destructor won't close our fd

	debug(colour::BLUE, "Fd", "move constructor called: new object now holds fd " + std::to_string(_fd));
}

// Destructor
Fd::~Fd(void)
{
	if (_fd != -1)
	{
		debug(colour::BLUE, "Fd", "destructor called: fd " + std::to_string(_fd) + " is closed");
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

		debug(colour::BLUE, "Fd", "move assignment called: fd " + std::to_string(old_fd) + " is now closed and holds fd " + std::to_string(this->_fd));

	}
	return (*this);
}
