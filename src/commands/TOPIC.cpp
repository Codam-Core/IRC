#include "Commands.hpp"
#include "Server.hpp"

void Commands::TOPIC(Client &client, const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "TOPIC command called from fd " << client.get_fd() << " with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}