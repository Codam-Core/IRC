#pragma once

#include <iostream>
#include <string>


class Server
{
	private:
	// Member variables
		const size_t _port;
		const std::string _password;
		
		static constexpr const char *INVALID_PORT_MSG = "Invalid port; Give a port number between 1 and 65535.";
		static constexpr const char *INVALID_PASSWORD_MSG = "Invalid Password; Password can not be empty.";
		
		// Member functions
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
};

std::ostream& operator<<(std::ostream &output_stream, const Server& src);
