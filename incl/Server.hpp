#pragma once

#include <iostream>
#include <string>


class Server
{
	private:
	// Member variables
		const size_t _port;
		const std::string _password;
		
		std::string invalid_port_error_message = "Invalid port; Give a number between 1 and 65535.";
		std::string invalid_password_error_message = "Password can not be empty.";
		
		// Member functions
		std::string parse_password(const std::string &password_str);
		size_t parse_port(const std::string &port_str);

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
};

std::ostream& operator<<(std::ostream &output_stream, const Server& src);
