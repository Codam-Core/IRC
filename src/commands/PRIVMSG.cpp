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
	// PRIVMSG a,#b,c :text → one message per target
	std::vector<std::string> targets = split(args[0], ',');
	if (targets.empty())
	{
		stream.prefix().code(411).client(client).message("No recipient given (PRIVMSG)").finish();
		return;
	}
	for (const std::string &target : targets)
		send_privmsg(client, target, args[1], stream);
}


// send one PRIVMSG to one target: a channel (#...) or a nickname
void Commands::send_privmsg(Client &client, const std::string &target, const std::string &text, CommandStream& stream)
{
	CommandStream msg_stream("PRIVMSG", _server);
	msg_stream.prefix(client).command().parameter(target).message(text).finish();

	// check if the target for the privmsg is a channel
	if (target.front() == '#')
	{
		Channel *channel = _server.find_channel(target);
		if (channel == nullptr)
		{
			stream.prefix().code(403).client(client).parameter(target).message("No such channel").finish();
			return;
		}
		if (!channel->is_member(&client))
		{
			stream.prefix().code(404).client(client).parameter(target).message("Cannot send to channel").finish();
			return;
		}
		msg_stream.send_to_channel(*channel, &client);
		return;
	}

	// check if the target for the privmsg is a nickname
	Client *receiver = _server.fetch_client_by_nickname(target);
	if (receiver == nullptr)
	{
		stream.prefix().code(401).client(client).parameter(target).message("No such nick/channel").finish();
		return;
	}
	msg_stream.send(*receiver);
}
