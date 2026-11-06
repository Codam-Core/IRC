#include "Commands.hpp"

// Allows the server to show all channal privileges a user has instead of just one
#define MULIT_PREFIX ":mulit-prefix"

void Commands::CAP(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    print_info(client, "CAP", args);
    // if the client didn't provide any arguments, send back an error message
    if (args.empty())
    {
        not_enough_parameters(client, stream);
        return;
    }

    // fetch the subcommand
    const std::string& subcommand = args[0];

    // send back the list of capabilities the server supports
    if(subcommand == "LS")
    {
        stream.command().parameter("*").parameter("LS").parameter(MULIT_PREFIX).finish();
        return;
    }
    
    // send back a confirmation that the capability has been enabled
    if (subcommand == "REQ")
    {
        if (args.size() == 2 && args[1] == MULIT_PREFIX)
            stream.command().parameter("ACK").parameter(MULIT_PREFIX).finish();
        else if (args.size() == 2 && args[1] != MULIT_PREFIX)
            stream.command().parameter("*").parameter("NAK").parameter(args[1]).finish();
        return;
    }
    
    // the client has finished the capability negotiation, validate the client and send back a welcome message
    if (subcommand == "END")
    {
        // @TODO we should try to check if the client has the password correct nickname is valid etc..
        stream.prefix().code(1).client(client).message("Welocme to IRC server!").finish();
        stream.prefix().code(1).client(client).message("Your host is irc.JL").finish();
        return;
    }

    // Unkown subcommand, send back an error message
    stream.prefix().code(410).client(client).parameter(subcommand).message("Invalid CAP subcommand").finish();
}