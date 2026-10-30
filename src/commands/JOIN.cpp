#include "Commands.hpp"

void Commands::JOIN(const std::string &args)
{
    std::cout << colour::BLUE << "JOIN command called with arguments" << args << colour::RESET << std::endl;
}