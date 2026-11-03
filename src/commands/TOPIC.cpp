#include "Commands.hpp"

void Commands::TOPIC(const std::string &args)
{
    std::cout << colour::BLUE << "TOPIC command called with arguments" << args << colour::RESET << std::endl;
}