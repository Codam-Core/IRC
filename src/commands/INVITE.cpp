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

    // fetch the target client and the channel from the server
    Client* target = _server.fetch_client_by_nickname(args[0]);
    Channel* channel = _server.find_channel(args[1]);

    // check whether the nickname is known to the server. If not, send back an error message.
    if (target == nullptr)
    {
        stream.prefix().code(401).client(client).parameter(args[0]).message("Target nickname doesn't exist").finish();
        return;
    }

    // check whether the channel exists. If not, send back an error message.
    if (channel == nullptr)
    {
        stream.prefix().code(403).client(client).parameter(args[1]).message("Channel doesn't exist").finish();
        return;
    }

    // is the client a member of the channel? If not, send back an error message.
    if (!channel->is_member(&client))
    {
        stream.prefix().code(442).client(client).parameter(args[1]).message("You're not on that channel").finish();
        return;
    }

    if (channel->is_member(target))
    {
        stream.prefix().code(443).client(client).parameter(args[0]).parameter(args[1]).message("is already on channel").finish();
        return;
    }

    if (channel->has_mode(Channel::INVITE_ONLY) && !channel->is_operator(&client))
    {
        stream.prefix().code(482).client(client).parameter(args[1]).message("You're not channel operator").finish();
        return;
    }

    // add the target client to the channel's invited list
    channel->add_invite(target);

    // NOTIFY the target the client has invited them to the channel.
    stream.prefix().code(341).client(client).parameter(args[0]).parameter(args[1]).finish();

    // also send a message to the target user that they have been invited to the channel.
    CommandStream sendStream("INVITE", _server);
    sendStream.prefix(client).command().client(*target).message(args[1]).finish();
    sendStream.send(*target);
}