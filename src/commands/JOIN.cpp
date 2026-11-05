#include "Commands.hpp"
#include "Server.hpp"

void Commands::JOIN(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    std::cout << colour::BLUE << "JOIN command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}