#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "ConnectedSocket.hpp"

class Client
{
	private:
		// Member variables
		ConnectedSocket	_socket;
		std::string		_nickname;
		std::string		_username;
		bool			_has_password = false;
		bool			_is_registered = false;


	public:
		// Constructors
		Client(void) = delete;
		explicit Client(ConnectedSocket socket);
		Client(const Client& other) = delete;
		Client(Client&& other) noexcept = default;

		// Destructor
		~Client(void);

		// Operators
		Client &operator=(const Client& other) = delete;
		Client &operator=(Client&& other) noexcept = default;

		// Getters and Setters
		int get_fd() const;
		ConnectedSocket &get_socket();

		// Member functions
};

std::ostream& operator<<(std::ostream &output_stream, const Client& src);

// Exceptions
class ExceptionName : public std::exception
{
	const char* what() const noexcept override;
};

