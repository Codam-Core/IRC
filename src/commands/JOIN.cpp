#include "Commands.hpp"
#include "Server.hpp"

void Commands::JOIN(Client &client, const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "JOIN command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;


	
	if (args.empty())
	{
		std::cout << "JOIN: not enough parameters" << std::endl;   // later: 461 ERR_NEEDMOREPARAMS
		return;
	}

	Channel &channel = _server.get_or_create_channel(args[0]);
	std::cout << "JOIN: channel that has joined is: " << channel.get_name() << std::endl;

}
