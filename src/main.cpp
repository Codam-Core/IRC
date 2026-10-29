#include "Server.hpp"

static constexpr const char *PROGRAM_USAGE_MSG = "Run this program as follows: ./ircserv <port> <password>";
volatile sig_atomic_t g_stop = 0;

static void handle_signal(int signal)
{
	(void)signal;
	g_stop = 1;
}


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

		signal(SIGINT, handle_signal);

		while(g_stop == 0)
		{
			server.update();
		}
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
