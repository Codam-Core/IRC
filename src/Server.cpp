#include "Server.hpp"

// Constructor
Server::Server(const std::string &port, const std::string &password) : _port(parse_port(port)), _password(parse_password(password))
{
	std::cout << "Server parameterized constructor is called" << std::endl;
}

// Destructor
Server::~Server(void)
{
	std::cout << "Server destructor is called" << std::endl;
}

// Getters and Setters

size_t Server::get_port() const
{
	return (_port);
}

std::string Server::get_password() const
{
	return (_password);
}

// Member functions
static std::string Server::parse_password(const std::string &password_str)
{
	if (password_str == "")
	{
		throw std::invalid_argument("Password can not be empty.");
	}
	else
	{
		return (password_str);
	}
}

static size_t Server::parse_port(const std::string &port_str)
{
	std::string error_message = "Invalid port; Give a number between 1 and 65535.";

	if (port_str.length() == 0 || port_str.length() > 5)
	{
		throw std::invalid_argument(error_message);
	}

	for (size_t i = 0; i < port_str.length(); i++)
	{
		if (!isdigit(static_cast<unsigned char>(port_str[i])))
		{
			throw std::invalid_argument(error_message);
		}
	}

	int port_nbr = std::stoi(port_str);
	if (port_nbr < 1 || port_nbr > 65535)
	{
		throw std::invalid_argument(error_message);
	}
	else 
	{
		return ((size_t)port_nbr);
	}
}

// Operators
std::ostream& operator<<(std::ostream& output_stream, const Server& src)
{
	output_stream << "parsed port = " << src.get_port() << std::endl;
	output_stream << "parsed password = " << src.get_password() << std::endl;

	return (output_stream);
}

// Exceptions
const char* ExceptionName::what() const noexcept
{
	return ("*Exception info here*");
}

