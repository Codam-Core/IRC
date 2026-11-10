#include "Commands.hpp"
#include "Server.hpp"

void Commands::PASS(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    if (args.empty())
    {
        not_enough_parameters(client, stream);
        return;
    }

	if (client.is_registered())
    {
        stream.prefix().code(462).client(client).message("You may not reregister").finish();
        return;
    }

    if (!_server.check_password(client, args[0]))
    {
        stream.prefix().code(464).client(client).message("Password incorrect").finish();
		debug(colour::CYAN, "PASS", "fd " + std::to_string(client.get_fd()) + ": wrong password, closing");
        return;
    }
    // no repsonse is send when a password is accepted.
	debug(colour::CYAN, "PASS", "fd " + std::to_string(client.get_fd()) + ": password ok");
}