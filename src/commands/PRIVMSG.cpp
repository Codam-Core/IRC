#include "Commands.hpp"
#include "Server.hpp"

void Commands::PRIVMSG(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    std::cout << colour::BLUE << "PRIVMSG command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}