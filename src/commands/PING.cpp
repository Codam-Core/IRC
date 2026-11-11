#include "Commands.hpp"


void Commands::PING(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
	if (args.empty())
	{
		not_enough_parameters(client, stream);
		return ;
	}

	stream.prefix().parameter("PONG").parameter("irc.JL").message(args[0]).finish();
}
