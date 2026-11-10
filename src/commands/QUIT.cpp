#include "Commands.hpp"
#include "Server.hpp"

void Commands::QUIT(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    // QUIT [:reason]
    std::string reason = "Quit";
    if (!args.empty() && !args[0].empty())
        reason = "Quit: " + args[0];

    // tell everyone who shares a channel, and leave all channels right away
    _server.leave_all_channels(client, reason);

    // goodbye message to the client itself; the connection closes after it has been sent
    stream.parameter("ERROR").message("Closing link (" + reason + ")").finish();
	debug(colour::CYAN, "QUIT", client.get_nickname() + " quit (" + reason + ")");
    client.mark_for_close();
}
