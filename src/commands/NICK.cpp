#include "Commands.hpp"

void Commands::NICK(const std::string &args)
{
    std::cout << colour::BLUE << "NICK command called with arguments" << args << colour::RESET << std::endl;
}