#include "Commands.hpp"
#include "Server.hpp"

const std::map<Commands::Command, Commands::CommandFn> Commands::_commands = {
    {"CAP", &Commands::CAP},
    {"PASS", &Commands::PASS},
    {"NICK", &Commands::NICK},
    {"JOIN", &Commands::JOIN},
    {"USER", &Commands::USER},
    {"MODE", &Commands::MODE},
    {"TOPIC", &Commands::TOPIC},
    {"INVITE", &Commands::INVITE},
    {"PRIVMSG", &Commands::PRIVMSG},
    {"PING", &Commands::PING},
    {"KICK", &Commands::KICK},
    {"QUIT", &Commands::QUIT}
};

Commands::Commands(Server& server) : _server(server) {};

void Commands::not_enough_parameters(Client &client, CommandStream& stream)
{
    stream.prefix().code(461).client(client).command().message("Not enough parameters").finish();
}

void Commands::try_register(Client &client, CommandStream& stream)
{
    // already registered, or NICK / USER still missing: nothing to do yet
    if (client.is_registered() || client.get_nickname().empty() || client.get_username().empty())
        return;

    // NICK and USER are done, but no (correct) PASS was given
    if (client.get_password().empty())
    {
        stream.prefix().code(464).client(client).message("Password incorrect").finish();
        client.mark_for_close();
        return;
    }

    client.set_registered();
    stream.prefix().code(1).client(client).message("Welcome to the IRC network " + client.get_nickname() + "!" + client.get_username() + "@" + client.get_hostname()).finish();
}

void Commands::print_info(Client &client, const std::string& cmd, const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << cmd << " command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}

void Commands::register_mode_change(const std::string& mode, bool adding, std::optional<std::string> additional_argument)
{
    _mode_changes.push_back({mode, adding, additional_argument});
}

void Commands::broadcast_mode_changes(Channel* channel, Client& client)
{
    // If there are no mode changes to broadcast, return early.
    if (_mode_changes.empty())
        return;

    // Construct the mode string and parameters for the MODE command.
    std::string mode_str;
    std::vector<std::string> mode_params;
    bool adding = false;

    // Determine whether the first mode change is adding or removing modes.
    for (const auto& change : _mode_changes)
    {
        // check if we have to add a '+' or '-' to the mode string
        // we always add a '+' or '-' when the first mode is added, and we also add it when the adding state changes (from adding to removing or vice versa)
        if (mode_str.empty() || adding != change.adding)
            mode_str += (change.adding ? "+" : "-");

        // add the mode character to the mode string
        mode_str += change.mode;
        
        // if the mode change has an additional argument, add it to the parameters
        if (change.additional_argument)
            mode_params.push_back(*change.additional_argument);
    }

    CommandStream stream("MODE", _server);
    stream.prefix(client).command().parameter(channel->get_name()).parameter(mode_str).parameter(mode_params).finish();

    // broadcase the mode changes to all members of the channel. including the requesting client.
    stream.send_to_channel(*channel);

	// start empty for the next MODE command
    _mode_changes.clear();
}

void Commands::handle_command(Client &client, const std::string& cmd, const std::vector<std::string>& args)
{
    // create a command stream with the command name
    CommandStream stream(cmd, _server);

    // fetch the cmd pair
    auto it = _commands.find(cmd);

    // we couldn't find the cmd
    if (it == _commands.end())
    {
        std::cerr << colour::RED << "[Commands] " << colour::RESET << "unknown command: " << cmd << std::endl; // temp error message; later this becomes 421 ERR_UNKNOWNCOMMAND, sent back to that client.
        return;
    }

	// before registration, only these commands are allowed
	if (!client.is_registered() && cmd != "CAP" && cmd != "PASS" && cmd != "NICK" && cmd != "USER" && cmd != "PING" && cmd != "QUIT")
	{
        stream.prefix().code(451).client(client).message("You have not registered").finish();
        stream.send(client);
        return;
    }

    // fetch the function pointer
    const CommandFn& command = it->second;
    
    // this syntax is a bit weird but what happends is that since command is a member function of Command class
    // we also have to call it with the "this" keyword so that the method has the class object (this). and ofcourse we have to pass (args)
    (this->*command)(client, args, stream);

    // send the data to the client
    stream.send(client);
}