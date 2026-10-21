
#include <iostream>
#include <string>
#include "Server.hpp"
#include "Fd.hpp"



#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <fcntl.h>
#include <netinet/in.h>
#include <cctype>
#include <cstdlib>
#include <cstdint>


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

		Fd my_fd(socket(AF_INET, SOCK_STREAM, 0));
		std::cout << my_fd << std::endl;

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