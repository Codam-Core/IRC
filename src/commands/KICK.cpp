#include "Commands.hpp"
#include "Server.hpp"

void Commands::KICK(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    print_info(client, "KICK", args);

    // KICK <channel> <nick> [:reason]
    if (args.size() < 2)
    {
        not_enough_parameters(client, stream);
        return;
    }

    // does the channel exist?
    Channel* channel = _server.find_channel(args[0]);
    if (channel == nullptr)
    {
        stream.prefix().code(403).client(client).parameter(args[0]).message("No such channel").finish();
        return;
    }

    // is the kicker in the channel?
    if (!channel->is_member(&client))
    {
        stream.prefix().code(442).client(client).parameter(args[0]).message("You're not on that channel").finish();
        return;
    }

    // only channel operators may kick
    if (!channel->is_operator(&client))
    {
        stream.prefix().code(482).client(client).parameter(args[0]).message("You're not channel operator").finish();
        return;
    }

    // is the target in the channel?
    Client* target = _server.fetch_client_by_nickname(args[1]);
    if (target == nullptr || !channel->is_member(target))
    {
        stream.prefix().code(441).client(client).parameter(args[1]).parameter(args[0]).message("They aren't on that channel").finish();
        return;
    }

    // the reason is optional; the default is the kicker's nick
    std::string reason = client.get_nickname();
    if (args.size() >= 3 && !args[2].empty())
        reason = args[2];

    // tell everyone in the channel, INCLUDING the target, so their client closes the channel window
    CommandStream kick_stream("KICK", _server);
    kick_stream.prefix(client).command().parameter(channel->get_name()).parameter(target->get_nickname()).message(reason).finish();
    kick_stream.send_to_channel(*channel);

    // remove the target LAST: this can erase the channel, so don't use `channel` after this line
    _server.remove_client_from_channel(*target, *channel);
}
