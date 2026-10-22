#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "Fd.hpp"

class ListeningSocket
{
	private:
		// Member variables
		Fd _fd;

		// Error messages
		static constexpr const char *FAILED_TO_CREATE_SOCKET_MSG = "Failed to create socket: ";
		static constexpr const char *FAILED_TO_SET_SOCKET_OPTION_MSG = "Failed to set socket options: ";
		static constexpr const char *FAILED_TO_SET_NONBLOCKING_MSG = "Failed to set socket to non-blocking: ";
		static constexpr const char *FAILED_TO_BIND_SOCKET_MSG = "Failed to bind socket on port ";
		static constexpr const char *FAILED_TO_LISTEN_MSG = "Failed to listen on socket: ";
		static constexpr const char *FAILED_TO_ACCEPT_MSG = "Failed to accept client: ";

	public:
		// Constructors
		ListeningSocket(void) = delete;
		explicit ListeningSocket(uint16_t port);
		ListeningSocket(const ListeningSocket& other) = delete;

		// Destructor
		~ListeningSocket(void);

		// Operators
		ListeningSocket &operator=(const ListeningSocket& other) = delete;

		// Getters and Setters
		int get_fd() const ;

		// Member functions
		Fd accept_client();
};
