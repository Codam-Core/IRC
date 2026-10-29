#pragma once

#include "includes.hpp"
#include "colours.hpp"

class Fd
{
	private:
		int _fd;

	public:
		// Constructors
		Fd(void) = delete;
		explicit Fd(int fd); // explicit is very important here, because it stops an int from silently turning into a temporary Fd, whose destructor would close() that fd
		
		// Copy constructor
		Fd(const Fd& other) = delete;

		// Move constructor
		Fd(Fd&& other) noexcept; // noexcept is important here, because otherwise vectors will try to copy it and thats not allowed!

		// Destructor
		~Fd(void);

		// Operators
		Fd &operator=(const Fd& other) = delete;
		Fd &operator=(Fd&& other) noexcept;

		// Getters and Setters
		int get_fd() const ;
};
