#include "Commands.hpp"

void Commands::NICK(const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "NICK command called with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}