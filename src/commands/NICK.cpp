#include "Commands.hpp"
#include "Server.hpp"

void Commands::NICK(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
	print_info(client, "NICK", args);

	// no nickname given: "NICK" or "NICK :"
	if (args.empty() || args[0].empty())
	{
		stream.prefix().code(431).client(client).message("No nickname given").finish();
		return;
	}

	// check whether the nickname is already in use
	if (_server.nickname_taken(args[0]))
		return stream.prefix().code(433).client(client).parameter(args[0]).message("Nickname is already in use").finish();

	// confirm a nick change, only when registered. built BEFORE set_nickname, so the prefix still has the old nick (:old!user@host NICK :new)
	// @TODO maybe send the change to all other clients as well, so they know the nickname has changed
	if (client.is_registered())
		stream.prefix(client).command().message(args[0]).finish();
	
	// set the nickname for the client
	client.set_nickname(args[0]);
	try_register(client, stream);
}
