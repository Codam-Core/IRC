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
		std::string		_realname;
		std::string		_hostname = "localhost";
		std::string     _password;
		bool			_is_registered = false;
		bool 			_close_after_send = false;   // "close me as soon as everything is sent"


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
		void set_nickname(const std::string &nickname);
		void set_username(const std::string &username);
		void set_hostname(const std::string &hostname);
		void set_password(const std::string &password);
		void set_realname(const std::string &realname);
		std::string get_nickname() const;
		std::string get_username() const;
		std::string get_hostname() const;
		std::string get_password() const;
		std::string get_realname() const;

		bool is_registered() const;
		void set_registered();

		void mark_for_close();
		bool is_marked_for_close() const;
		
		// Operator overloaders
		bool operator==(const Client &other) const;
		bool operator!=(const Client &other) const;
};

