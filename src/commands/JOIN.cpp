#include "Commands.hpp"
#include "Server.hpp"

// JOIN #a,#b,#c key1,key2: join every channel in the list, with the key at the same position
void Commands::JOIN(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
	std::vector<std::string> channels;

	// check whether the client gave the required arguments
	if (!args.empty())
		channels = split(args[0], ',');

	if (channels.empty())
	{
		not_enough_parameters(client, stream);
		return;
	}

	std::vector<std::string> keys;
	if (args.size() >= 2)
		keys = split(args[1], ',');

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string key = "";
		if (i < keys.size())
			key = keys[i];
		join_channel(client, channels[i], key, stream);
	}
}

void Commands::join_channel(Client &client, const std::string &name, const std::string &key, CommandStream& stream)
{
	// check if the channel name starts with #
	if (name.empty() || name.front() != '#')
	{
		stream.prefix().code(403).client(client).parameter(name).message("No such channel").finish();
		return;
	}

	// name is just "#"
	if (name.size() < 2)
	{
		stream.prefix().code(403).client(client).parameter(name).message("No such channel").finish();
		return;
	}

	// check if the channel already exsists
	bool new_channel = _server.find_channel(name) == nullptr;

	// if channel is new: create it, if not: get it
	Channel &channel = _server.get_or_create_channel(name);

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
	if (channel.has_mode(Channel::PASSWORD) && key != channel.get_key())
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

	debug(colour::CYAN, "JOIN", client.get_nickname() + " joined " + channel.get_name() + (new_channel ? " (new channel, operator)" : ""));

	// send message to everyone in the channel that someone joined
	
	// What a client expects after its JOIN line
	// :liath!liath@localhost JOIN :#a                 ← to everyone (you have this)
	// :irc.JL 332 liath #a :the topic                 ← only if there's a topic
	// :irc.JL 353 liath = #a :@liath jisse            ← who's in it, @ = operator
	// :irc.JL 366 liath #a :End of /NAMES list        ← end of the list

	CommandStream join_stream("JOIN", _server);
	join_stream.prefix(client).command().message(channel.get_name()).finish();
	join_stream.send_to_channel(channel);

	// the topic, only if there is one
	if (!channel.get_topic().empty())
		stream.prefix().code(332).client(client).parameter(channel.get_name()).message(channel.get_topic()).finish();

	// the names list: everyone in the channel, '@' in front of operators
	stream.prefix().code(353).client(client).parameter("=").parameter(channel.get_name()).get_member_names(channel).finish();
	stream.prefix().code(366).client(client).parameter(channel.get_name()).message("End of /NAMES list").finish();
}
