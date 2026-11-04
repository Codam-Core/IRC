#include "Commands.hpp"

void Commands::TOPIC(const std::vector<std::string>& args)
{
    std::cout << colour::BLUE << "TOPIC command called with arguments ";
    for (auto &argument : args)
    {
        std::cout << argument << ", ";
    }  
    std::cout << colour::RESET << std::endl;
}