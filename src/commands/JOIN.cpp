#include "Commands.hpp"
#include "Server.hpp"

void Commands::JOIN(Client &client, const std::vector<std::string>& args)
{
    std::cout << colour::YELLOW << "JOIN command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;



	if (args.empty())
	{
		std::cout << "461 not enough parameters" << std::endl; // change to jisses reply
		return;
	}
	if (args[0].front() != '#')
	{
		std::cout << "403 no such channel" << std::endl; // change to jisses reply
		return;
	}

	bool new_channel;
	if (_server.find_channel == nullptr)
		new_channel = true;
	else
		new_channel = false;
	
	Channel &channel = _server.get_or_create_channel(args[0]);
	std::cout << "JOIN: channel that has joined is: " << channel.get_name() << std::endl;

	if (!channel.is_member(client))
	{
		if (new_channel)
			channel.add_member(client, true);
		else
			channel.add_member(client, false);
	}
	// else client is already a member and we ignore it

	

	


}
