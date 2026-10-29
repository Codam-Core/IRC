#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "ListeningSocket.hpp"
#include "ConnectedSocket.hpp"
#include "Epoll.hpp"

extern volatile sig_atomic_t g_stop; // defined in main.cpp, set by the signal handler

class Server
{
	private:
		// Member variables
		const uint16_t 					_port; // a port has room for exactly 16 bits, hence the type uint16_t
		const std::string 				_password;

		ListeningSocket 				_listeningSocket;
		Epoll 							_epoll;
		
		std::map<int, ConnectedSocket>	_clients; // key = client's fd

		static constexpr int			MAX_EVENTS = 64;

		// Error messages
		static constexpr const char *INVALID_PORT_MSG = "Invalid port: Give a port number between 1 and 65535.";
		static constexpr const char *INVALID_PASSWORD_MSG = "Invalid Password: Password can not be empty.";

		static constexpr const char *FAILED_TO_WAIT_FOR_EPOLL_MSG = "Failed to wait for epoll events: ";

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
		void update();
};
