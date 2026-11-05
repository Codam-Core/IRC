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

std::string Client::get_nickname() const
{
	return (_nickname);
}

std::string Client::get_username() const
{
	return (_username);
}

std::string Client::get_hostname() const
{
	return (_hostname);
}

std::string Client::get_password() const
{
	return (_password);
}

std::string Client::get_realname() const
{
	return (_realname);
}

void Client::set_nickname(const std::string &nickname)
{
	_nickname = nickname;
}

void Client::set_username(const std::string &username)
{
	_username = username;
}

void Client::set_hostname(const std::string &hostname)
{
	_hostname = hostname;
}

void Client::set_password(const std::string &password)
{
	_password = password;
}

void Client::set_realname(const std::string &realname)
{
	_realname = realname;
}

// Operator overloaders
bool Client::operator==(const Client &other) const // becasue we define ==, the compiler writes != for us automatically.
{
	if (this->get_fd() == other.get_fd())   // two clients are "the same" if they have the same fd
		return (true);
	else
		return (false);
}

bool Client::operator!=(const Client &other) const
{
	if (this->get_fd() != other.get_fd())   // two clients are "the same" if they have the same fd
		return (true);
	else
		return (false);
}

