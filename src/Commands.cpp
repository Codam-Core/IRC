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


void Commands::handle_command(const std::string& cmd, const std::vector<std::string>& args)
{
    // fetch the cmd pair
    auto it = _commands.find(cmd);

    // we couldn't find the cmd
    if (it == _commands.end())
        throw std::invalid_argument("Invalid command");

    // fetch the function pointer
    const CommandFn& command = it->second;
    
    // this syntax is a bit weird but what happends is that since command is a member function of Command class
    // we also have to call it with the "this" keyword so that the method has the class object (this). and ofcourse we have to pass (args)
    (this->*command)(args);
}