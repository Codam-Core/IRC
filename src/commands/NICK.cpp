#include "Commands.hpp"
#include "Server.hpp"

void Commands::NICK(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    print_info(client, "NICK", args);

    // check whether the client gave the required arguments
    if (args.empty())
    {
        not_enough_parameters(client, stream);
        return;
    }

    // check whether the nickname is already in use
    if (_server.nickname_taken(args[0]))
        return stream.prefix().code(433).client(client).parameter(args[0]).message("Nickname is already in use").finish();

    // set the nickname for the client
    client.set_nickname(args[0]);

    // send a confirmation message back to the client
    // @TODO maybe send the change to all other clients as well, so they know the nickname has changed
    stream.prefix(client).command().message(args[0]).finish();
}