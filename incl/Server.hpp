#pragma once

#include <iostream>
#include <string>


class Server
{
	private:
		// Member variables
		const size_t _port;
		const std::string _password;
		
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

// Exceptions
class ExceptionName : public std::exception
{
	const char* what() const noexcept override;
};

