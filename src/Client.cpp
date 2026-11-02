#include "Client.hpp"

// Constructors
Client::Client(ConnectedSocket socket) : _socket(std::move(socket))
{
	std::cerr << colour::BRIGHT_BLUE << "[Client] " << colour::RESET << "constructor called: client fd " << _socket.get_fd() << std::endl;
}

// Destructor
Client::~Client(void)
{
	std::cerr << colour::BRIGHT_BLUE << "[Client] " << colour::RESET << "destructor called: client fd " << _socket.get_fd() << std::endl;
}

// Getters and Setters
int Client::get_fd() const
{
	return (_socket.get_fd());
}

ConnectedSocket &Client::get_socket()
{
	return (_socket);
}

// Member functions


// Overload operators




// Exceptions


