#include "Commands.hpp"
#include "Server.hpp"

const std::map<Commands::Command, Commands::CommandFn> Commands::_commands = {
    {"NICK", &Commands::NICK},
    {"JOIN", &Commands::JOIN},
    {"TOPIC", &Commands::TOPIC},
    {"INVITE", &Commands::INVITE},
    {"PRIVMSG", &Commands::PRIVMSG},
};

Commands::Commands(Server& server) : _server(server) {};


void Commands::handle_command(Client &client, const std::string& cmd, const std::vector<std::string>& args)
{
    // fetch the cmd pair
    auto it = _commands.find(cmd);

    // we couldn't find the cmd
    if (it == _commands.end())
    {
        std::cerr << colour::RED << "[Commands] " << colour::RESET << "unknown command: " << cmd << std::endl; // temp error message; later this becomes 421 ERR_UNKNOWNCOMMAND, sent back to that client.
        return;
    }

    // fetch the function pointer
    const CommandFn& command = it->second;
    
    // this syntax is a bit weird but what happends is that since command is a member function of Command class
    // we also have to call it with the "this" keyword so that the method has the class object (this). and ofcourse we have to pass (args)
    (this->*command)(client, args);
}