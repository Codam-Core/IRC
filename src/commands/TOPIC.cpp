#include "Commands.hpp"
#include "Server.hpp"

void Commands::TOPIC(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    print_info(client, "TOPIC", args);

    if (args.empty())
    {
        not_enough_parameters(client, stream);
        return;
    }

    Channel* channel = _server.find_channel(args[0]);
    if (channel == nullptr)
    {
        stream.prefix().code(403).client(client).parameter(args[0]).message("No such channel").finish();
        return;
    }
    if (!channel->is_member(&client))
    {
        stream.prefix().code(442).client(client).parameter(args[0]).message("You're not on that channel").finish();
        return;
    }

    // does the client want to request the topic, or set a new one?
    if (args.size() == 1)
    {
        std::string topic = channel->get_topic();
        if (topic.empty())
            stream.prefix().code(331).client(client).parameter(args[0]).message("No topic is set").finish();
        else
            stream.prefix().code(332).client(client).parameter(args[0]).message(topic).finish();
        return;
    }

    // If topic lock is enabled and the client is not an operator, send an error message
    if (channel->has_mode(Channel::TOPIC_LOCK) && !channel->is_operator(&client))
    {
        stream.prefix().code(482).client(client).parameter(args[0]).message("You're not channel operator").finish();
        return;
    }

    // set the new topic
    channel->set_topic(args[1]);

    // send a confirmation to the client who set the topic
    CommandStream broadcastStream("TOPIC", _server);
    broadcastStream.prefix(client).command().parameter(channel->get_name()).message(channel->get_topic()).finish();

    // broadcast the new topic to all members of the channel including the client who set it
    for (auto &[other, is_operator] : channel->get_members())
    {
        broadcastStream.send(*other);
    }
}