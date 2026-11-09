#include "Commands.hpp"
#include "Server.hpp"

void Commands::JOIN(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
	// check whether the client gave the required arguments
	if (args.empty())
	{
		not_enough_parameters(client, stream);
		return;
	}

	// check if the channel name starts with #
	if (args[0].empty() || args[0].front() != '#')
	{
		stream.prefix().code(403).client(client).parameter(args[0]).message("No such channel").finish();
		return;
	}

	// check if the channel already exsists
	bool new_channel = _server.find_channel(args[0]) == nullptr;

	// if channel is new: create it, if not: get it
	Channel &channel = _server.get_or_create_channel(args[0]);

	// check if client is already a member: if so, do nothing
	if (channel.is_member(&client))
		return;

	// +i: only invited clients may join
	if (channel.has_mode(Channel::INVITE_ONLY) && !channel.is_invited(&client))
	{
		stream.prefix().code(473).client(client).parameter(channel.get_name()).message("Cannot join channel (+i)").finish();
		return;
	}

	// +k: the client must give the right key (JOIN #a <key>)
	if (channel.has_mode(Channel::PASSWORD) && (args.size() < 2 || args[1] != channel.get_key()))
	{
		stream.prefix().code(475).client(client).parameter(channel.get_name()).message("Cannot join channel (+k)").finish();
		return;
	}

	// +l: the channel may not be full
	if (channel.has_mode(Channel::LIMIT) && channel.member_count() >= channel.get_limit())
	{
		stream.prefix().code(471).client(client).parameter(channel.get_name()).message("Cannot join channel (+l)").finish();
		return;
	}

	// join; if it's a new channel, the client becomes operator. an invite can only be used once
	channel.add_member(&client, new_channel);
	channel.remove_invite(&client);

	// send message to everyone in the channel that someone joined
	CommandStream join_stream("JOIN", _server);
	join_stream.prefix(client).command().message(channel.get_name()).finish();
	join_stream.send_to_channel(channel);
}
