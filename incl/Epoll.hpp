#pragma once

#include "includes.hpp"
#include "colours.hpp"
#include "Fd.hpp"

class Epoll
{
	private:
		// Member variables
		Fd _epoll_fd;

		// Error messages
		static constexpr const char *FAILED_TO_CREATE_EPOLL_MSG = "Failed to create epoll: ";
		static constexpr const char *FAILED_TO_ADD_FD_TO_EPOLL_MSG = "Failed to add fd to epoll: ";

	public:
		// Constructors
		Epoll(void);
		Epoll(const Epoll& other) = delete;

		// Destructor
		~Epoll(void);

		// Operators
		Epoll &operator=(const Epoll& other) = delete;

		// Getters and Setters
		int get_fd() const;

		// Member functions
		void add(int fd);
		void remove(int fd);
		int wait(epoll_event *events, int max_events);
};
