#include "Commands.hpp"
#include "Server.hpp"

void Commands::NICK(Client &client, const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "NICK command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}