#include "Commands.hpp"
#include "Server.hpp"

void Commands::JOIN(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
	if (args.empty())
	{
		not_enough_parameters(client, stream);
		return;
	}
	if (args[0].empty() || args[0].front() != '#')
	{
		stream.prefix().code(403).client(client).parameter(args[0]).message("No such channel").finish();
		return;
	}

	bool new_channel = _server.find_channel(args[0]) != nullptr;

	Channel &channel = _server.get_or_create_channel(args[0]);
	

	if (channel.is_member(&client))
		return;
	else
		channel.add_member(&client, new_channel);

	// TODO change this to stream 
	std::string join_message = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_hostname() + " JOIN " + channel.get_name() + "\r\n";

	for (auto &member : channel.get_members())
	{
		// sendstream.send()
		_server.send_to_client(*member.first, join_message);
	}
}

