#include "Commands.hpp"
#include "Server.hpp"

void Commands::PRIVMSG(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
	if (args.empty())
	{
		stream.prefix().code(411).client(client).message("No recipient given (PRIVMSG)").finish();
		return;
	}
	if (args.size() < 2 || args[1].empty())
	{
		stream.prefix().code(412).client(client).message("No text to send").finish();
		return;
	}

	CommandStream msg_stream("PRIVMSG", _server);
	msg_stream.prefix(client).command().parameter(args[0]).message(args[1]).finish();

	// check if the target for the privmsg is a channel
	if (args[0].front() == '#')
	{
		Channel *channel = _server.find_channel(args[0]);
		if (channel == nullptr)
		{
			stream.prefix().code(403).client(client).parameter(args[0]).message("No such channel").finish();
			return;
		}
		if (!channel->is_member(&client))
		{
			stream.prefix().code(404).client(client).parameter(args[0]).message("Cannot send to channel").finish();
			return;
		}
		msg_stream.send_to_channel(*channel, &client);
		return;
	}

	// check if the target for the privmsg is a nickname
	Client *target = _server.fetch_client_by_nickname(args[0]);
	if (target == nullptr)
	{
		stream.prefix().code(401).client(client).parameter(args[0]).message("No such nick/channel").finish();
		return;
	}
	msg_stream.send(*target);

}