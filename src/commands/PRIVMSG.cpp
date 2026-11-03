#include "Commands.hpp"

void Commands::PRIVMSG(const std::string &args)
{
    std::cout << colour::BLUE << "PRIVMSG command called with arguments" << args << colour::RESET << std::endl;
}