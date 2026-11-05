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

void Client::mark_for_close()
{
	_close_after_send = true;
}

bool Client::is_marked_for_close() const
{
	return (_close_after_send);
}

// Operator overloaders
bool Client::operator==(const Client &other) const // becasue we define ==, the compiler writes != for us automatically.
{
	if (this->get_fd() == other.get_fd())   // two clients are "the same" if they have the same fd
		return (true);
	else
		return (false);
}

