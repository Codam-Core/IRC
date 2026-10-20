#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <fcntl.h>

class Server
{
	private:
		// Member variables
		const size_t _port;
		const std::string _password;

		int _socket_fd;

		// Error messages
		static constexpr const char *INVALID_PORT_MSG = "Invalid port: Give a port number between 1 and 65535.";
		static constexpr const char *INVALID_PASSWORD_MSG = "Invalid Password: Password can not be empty.";
		static constexpr const char *FAILED_TO_CREATE_SOCKET_MSG = "Failed to create socket: ";
		static constexpr const char *FAILED_TO_SET_SOCKET_OPTION_MSG = "Failed to set socket options: ";
		static constexpr const char *FAILED_TO_SET_NONBLOCKING_MSG = "Failed to set socket to non-blocking: ";
		static constexpr const char *FAILED_TO_BIND_SOCKET_MSG = "Failed to bind socket on port [port, put in throw message]";
		static constexpr const char *FAILED_TO_LISTEN_MSG = "Failed to listen on socket: ";

		// Private member functions
		static std::string parse_password(const std::string &password_str);
		static size_t parse_port(const std::string &port_str);


	public:
		// Constructors
		Server(const std::string &port, const std::string &password);

		Server(const Server& other) = delete;

		// Destructor
		~Server(void);

		// Operators
		Server &operator=(const Server& other) = delete;

		// Getters and Setters
		size_t get_port() const;
		std::string get_password() const;
		int get_socket_fd() const;

		// Public member functions
		void create_socket();
};

std::ostream& operator<<(std::ostream &output_stream, const Server& src);
