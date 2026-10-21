#include "Fd.hpp"

// Constructors
Fd::Fd(void) : _fd(-1)
{
	std::cout << colour::BLUE << "Fd default constructor is called" << colour::RESET << std::endl;
}

Fd::Fd(int fd) : _fd(fd)
{
	std::cout << colour::BLUE << "Fd parameterized constructor is called" << colour::RESET << std::endl;
}

// Move constructor
Fd::Fd(Fd&& other) noexcept : _fd(other._fd)
{
	other._fd = -1;
	std::cout << colour::BLUE << "Fd moving constructor is called" << colour::RESET << std::endl;
}

// Destructor
Fd::~Fd(void)
{
	if (_fd != -1)
		close(_fd);

	std::cout << colour::BLUE << "Fd destructor is called" << colour::RESET << std::endl;
}

// Getters and Setters
int Fd::get_fd() const
{
	return (_fd);
}

// Overload operators
Fd& Fd::operator=(Fd&& other) noexcept
{
	if (this != &other)
	{
		if (this->_fd != -1)
			close(_fd);
		this->_fd = other._fd;
		other._fd = -1;
		std::cout << colour::BLUE << "Fd move assignment is called" << colour::RESET << std::endl;
	}
	return (*this);
}

std::ostream& operator<<(std::ostream& output_stream, const Fd& src)
{
	output_stream << "Fd number = " << src.get_fd();
	return (output_stream);
}
