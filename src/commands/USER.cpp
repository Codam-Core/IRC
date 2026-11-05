#include "Commands.hpp"
#include "Server.hpp"

void static print_info(Client &client, const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "USER command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}

void Commands::USER(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    print_info(client, args);

    // check whether the client gave the required arguments
    if (args.size() < 4)
    {
        not_enough_parameters(client, stream);
        return;
    }

    // check if the client is already registered
    if (_server.already_registered(client, args[0]))
    {
        stream.prefix().code(462).client(client).message("You may not reregister").finish();
        return;
    }

    // set the username and realname for the client
    client.set_username(args[0]);
    // args[1] is the user mode bitmask that we don't support
    // args[2] is an unused parameter
    client.set_realname(args[3]);
}