#include "Commands.hpp"
#include "Server.hpp"

void static print_info(Client &client, const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "INVITE command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}

void Commands::INVITE(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    print_info(client, args);
    if (args.size() < 2)
    {
        not_enough_parameters(client, stream);
        return;
    }

    Client* target = _server.fetch_client_by_nickname(args[0]);

    // check whether the nickname is known to the server. If not, send back an error message.
    if (target == nullptr)
    {
        stream.prefix().code(401).client(client).parameter(args[0]).message("Target nickname doesn't exist").finish();
        return;
    }

    // @TODO: check whether the channel exists
    // @TODO: check whether the client is a member of the channel
    // @TODO: check whether the target user is already a member of the channel
    // @TODO: check whether the channel is invite-only and whether the client has the privileges to invite others

    // NOTIFY the target the client has invited them to the channel.
    stream.prefix().code(341).client(client).parameter(args[0]).parameter(args[1]).finish();

    // also send a message to the target user that they have been invited to the channel.
    CommandStream sendStream("INVITE", _server);
    sendStream.prefix(client).command().client(*target).message(args[1]).finish();
    sendStream.send(*target);
}