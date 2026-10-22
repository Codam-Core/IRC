#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "ListeningSocket.hpp"

class Server
{
	private:
		// Member variables
		const uint16_t 		_port; // a port has room for exactly 16 bits, hence the type uint16_t
		const std::string 	_password;

		ListeningSocket 	_listeningSocket;
		Fd 					_epoll_fd;

		// Error messages
		static constexpr const char *INVALID_PORT_MSG = "Invalid port: Give a port number between 1 and 65535.";
		static constexpr const char *INVALID_PASSWORD_MSG = "Invalid Password: Password can not be empty.";

		static constexpr const char *FAILED_TO_CREATE_EPOLL_MSG = "Failed to create epoll: ";
		static constexpr const char *FAILED_TO_ADD_CLIENT_TO_EPOLL_MSG = "Failed to connect client to epoll: ";
		// Private member functions
		static std::string parse_password(const std::string &password_str);
		static uint16_t parse_port(const std::string &port_str);

	public:
		// Constructors
		Server(const std::string &port, const std::string &password);

		Server(const Server& other) = delete;

		// Destructor
		~Server(void);

		// Operators
		Server &operator=(const Server& other) = delete;

		// Getters and Setters
		uint16_t get_port() const;
		std::string get_password() const;

		// Public member functions
};
