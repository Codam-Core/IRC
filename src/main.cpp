
#include <iostream>
#include <string>
#include "Server.hpp"

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
		Server server(argv[1], argv[2]);
		server.create_socket();

		std::cout << server;
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