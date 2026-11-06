#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "ListeningSocket.hpp"
#include "ConnectedSocket.hpp"
#include "Epoll.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "Channel.hpp"
#include "utils.hpp"

extern volatile sig_atomic_t g_stop; // defined in main.cpp, set by the signal handler

class Server
{
	private:
		// Member variables
		const uint16_t 					_port; // a port has room for exactly 16 bits, hence the type uint16_t
		const std::string 				_password;

		ListeningSocket 				_listeningSocket;
		Epoll 							_epoll;
		
		std::map<int, Client>			_clients; // key = client's fd
		std::map<std::string, Channel>	_channels; // key = channel's name; lowercase with '#'

		Commands						_commands;

		static constexpr int			MAX_EVENTS = 64;

		// Error messages
		static constexpr const char *INVALID_PORT_MSG = "Invalid port: Give a port number between 1 and 65535.";
		static constexpr const char *INVALID_PASSWORD_MSG = "Invalid Password: Password can not be empty.";

		static constexpr const char *FAILED_TO_WAIT_FOR_EPOLL_MSG = "Failed to wait for epoll events: ";

		// Private member functions
		static std::string parse_password(const std::string &password_str);
		static uint16_t parse_port(const std::string &port_str);
		void disconnect_client(Client &client);
		bool read_from_client(Client &client); // return false? -> client disconnected
		void parse_command(Client &client, const std::string &line);

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
		bool check_password(Client& client, const std::string &password);
		
		// Client functions
		void send_to_client(Client &client, const std::string &message);
		bool nickname_taken(const std::string &nickname) const;
		bool already_registered(Client &client, const std::string& user_name) const;
		Client* fetch_client_by_nickname(const std::string &nickname) const;

		// Public member functions
		void update();

		// Helpers
		Channel *find_channel(const std::string &name);
		Channel &get_or_create_channel(const std::string &name);
};
