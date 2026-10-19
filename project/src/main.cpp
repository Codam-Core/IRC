
#include <iostream>
#include <string>
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Run this program as follows: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}

	try
	{
		Server server(argv[1], argv[2]);
		std::cout << server << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "An error occurred: " << e.what() << std::endl;
		std::cerr << "Run this program as follows: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}