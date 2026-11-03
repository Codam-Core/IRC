#include "Commands.hpp"

void Commands::JOIN(const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "JOIN command called with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}