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

	bool new_channel = _server.find_channel(args[0]) == nullptr;

	Channel &channel = _server.get_or_create_channel(args[0]);
	
	if (channel.is_member(&client))
		return;
	else
		channel.add_member(&client, new_channel);

	CommandStream join_stream("JOIN", _server);
	join_stream.prefix(client).command().message(channel.get_name()).finish();
	join_stream.send_to_channel(channel);
}
