#include "Commands.hpp"

void Commands::PRIVMSG(const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "PRIVMSG command called with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}