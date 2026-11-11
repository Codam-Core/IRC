#include "Commands.hpp"
#include "Server.hpp"

// PART #a,#b [:reason]: leave every channel in the list
void Commands::PART(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    std::vector<std::string> channels;
    if (!args.empty())
        channels = split(args[0], ',');

    if (channels.empty())
    {
        not_enough_parameters(client, stream);
        return;
    }

    // the reason is optional
    std::string reason = "";
    if (args.size() >= 2)
        reason = args[1];

    for (const std::string &name : channels)
        part_channel(client, name, reason, stream);
}

// leave one channel: tell everyone in it (also the client), then remove the client
void Commands::part_channel(Client &client, const std::string &name, const std::string &reason, CommandStream& stream)
{
    Channel *channel = _server.find_channel(name);
    if (channel == nullptr)
    {
        stream.prefix().code(403).client(client).parameter(name).message("No such channel").finish();
        return;
    }
    if (!channel->is_member(&client))
    {
        stream.prefix().code(442).client(client).parameter(name).message("You're not on that channel").finish();
        return;
    }

    CommandStream part_stream("PART", _server);
    if (reason.empty())
        part_stream.prefix(client).command().message(channel->get_name()).finish();
    else
        part_stream.prefix(client).command().parameter(channel->get_name()).message(reason).finish();
    part_stream.send_to_channel(*channel);

    // remove LAST: this can erase the channel, so don't use `channel` after this line
	debug(colour::CYAN, "PART", client.get_nickname() + " left " + channel->get_name());
    _server.remove_client_from_channel(client, *channel);
}
