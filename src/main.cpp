#include <iostream>
#include <string>
#include "Server.hpp"
#include "Fd.hpp"

static constexpr const char *PROGRAM_USAGE_MSG = "Run this program as follows: ./ircserv <port> <password>";

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << PROGRAM_USAGE_MSG << std::endl;
		return (EXIT_FAILURE);
	}

	try
	{
		// testing classes
		Server server(argv[1], argv[2]);
		std::cout << server << std::endl;

		// Fd my_fd(socket(AF_INET, SOCK_STREAM, 0));
		// // std::cout << "my_fd: " << my_fd << std::endl;

		// Fd other_fd(std::move(my_fd));

		// std::cout << "OG fd: " << my_fd << std::endl;
		// std::cout << "moved fd: " << other_fd << std::endl;

		Fd a(socket(AF_INET, SOCK_STREAM, 0));   // fd 3
		std::cout << "fd a: " << a << std::endl;

		Fd b(socket(AF_INET, SOCK_STREAM, 0));   // fd 4
		std::cout << "fd b: " << b << std::endl;

		a = std::move(b);
		std::cout << "fd a: " << a << std::endl;
		std::cout << "fd b: " << b << std::endl;

	}
	catch (const std::exception &e)
	{
		if (!e.what())
		{
			std::cerr << PROGRAM_USAGE_MSG << std::endl;
		}
		else
		{
			std::cerr << "An error occurred: " << e.what() << std::endl;
		}
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
