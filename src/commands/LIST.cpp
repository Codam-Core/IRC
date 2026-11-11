#include "Commands.hpp"
#include "Server.hpp"

// one line per channel: 322 <nick> <channel> <number of members> :<topic>
static void list_channel(Client &client, const Channel &channel, CommandStream &stream)
{
    stream.prefix().code(322).client(client).parameter(channel.get_name()).parameter(std::to_string(channel.member_count())).message(channel.get_topic()).finish();
}

// LIST: all channels. LIST #a,#b: only those channels
void Commands::LIST(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    if (args.empty() || args[0].empty())
    {
        for (auto &[name, channel] : _server.get_channels())
        {
            (void)name;
            list_channel(client, channel, stream);
        }
    }
    else
    {
        for (const std::string &name : split(args[0], ','))
        {
            Channel *channel = _server.find_channel(name);
            if (channel != nullptr)
                list_channel(client, *channel, stream);
        }
    }

    stream.prefix().code(323).client(client).message("End of /LIST").finish();
}
