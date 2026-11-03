#include "Commands.hpp"

void Commands::INVITE(const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "INVITE command called with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}