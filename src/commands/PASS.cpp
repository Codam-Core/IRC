#include "Commands.hpp"
#include "Server.hpp"

void static print_info(Client &client, const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "PASS command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}

void Commands::PASS(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    print_info(client, args);
    if (args.empty())
    {
        not_enough_parameters(client, stream);
        return;
    }

    if (!_server.check_password(client, args[0]))
    {
        stream.prefix().code(464).client(client).message("Password incorrect").finish();
        return;
    }
    // no repsonse is send when a password is accepted.
}