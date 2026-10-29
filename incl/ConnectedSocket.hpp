#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "Fd.hpp"

class ConnectedSocket
{
	private:
		// Member variables
		Fd _cs_fd;

		// Error messages
		static constexpr const char *FAILED_TO_SET_NONBLOCKING_MSG = "Failed to set client socket to non-blocking: ";

	public:
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
		ssize_t recv_data(char *buffer, size_t size);
};
