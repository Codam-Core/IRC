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
	
	// check if client is already a member, if so: were done. if not: make it a member. if its a new channel, make operator
	if (channel.is_member(&client))
		return;
	else
		channel.add_member(&client, new_channel);

	// send message to everyone in the channel that someone joined
	CommandStream join_stream("JOIN", _server);
	join_stream.prefix(client).command().message(channel.get_name()).finish();
	join_stream.send_to_channel(channel);
}
