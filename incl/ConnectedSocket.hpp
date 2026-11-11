#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "Fd.hpp"

class ConnectedSocket
{
	private:
		// Member variables
		Fd			_cs_fd;
		std::string	_recv_buffer; // bytes that don't form a complete line yet
		std::string	_send_buffer; // bytes waiting untill epoll says we can write

		static constexpr size_t RECV_SIZE = 512;

		// Error messages
		static constexpr const char *FAILED_TO_SET_NONBLOCKING_MSG = "Failed to set client socket to non-blocking: ";

	public:
		static constexpr size_t MAX_LINE_LENGTH = 510; // IRC: max 512 bytes per message, including \r\n

		// Constructors
		ConnectedSocket(void) = delete;
		explicit ConnectedSocket(Fd fd);

		// Copy constructor
		ConnectedSocket(const ConnectedSocket& other) = delete;

		// Move constructor
		ConnectedSocket(ConnectedSocket&& other) noexcept = default; // = default means "compiler, write the normal version for me". The normal version moves every member. Your only member is an Fd, and Fd already knows how to move. So this is all you need.

		// Destructor
		~ConnectedSocket(void);

		// Operators
		ConnectedSocket &operator=(ConnectedSocket&& other) noexcept = default;

		// Getters and Setters
		int get_fd() const ;

		// Member functions
		ssize_t recv_data();
		bool get_line(std::string &line);
		
		void queue_send(const std::string &message);
		ssize_t send_data();
		bool has_pending_send() const;
};
