#pragma once

#include "colours.hpp"
#include <iostream>
#include <unistd.h>
#include <utility>

class Fd
{
	private:
		int _fd;

	public:
		// Constructors
		Fd(void);
		explicit Fd(int fd); // explicit is very important here, because it stops an int from silently turning into a temporary Fd, whose destructor would close() that fd
		
		Fd(const Fd& other) = delete;
		Fd(Fd&& other) noexcept; // noexcept is important here, because otherwise vectors will try to copy it and thats not allowed!

		// Destructor
		~Fd(void);

		// Operators
		Fd &operator=(const Fd& other) = delete;
		Fd &operator=(Fd&& other) noexcept;

		// Getters and Setters
		int get_fd() const ;
};

std::ostream& operator<<(std::ostream &output_stream, const Fd& src);
