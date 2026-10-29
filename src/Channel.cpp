#include "Channel.hpp"
#include "colours.hpp"
#include <algorithm>

Channel::Channel(const std::string& name) : _name(name)
{
    std::cerr << colour::BLUE << "[Channel] " << colour::RESET << "constructor called: channel " << _name << " created" << std::endl;
}

Channel::~Channel(void)
{
    std::cerr << colour::BLUE << "[Channel] " << colour::RESET << "destructor called: channel " << _name << " destroyed" << std::endl;
}

int Channel::user_count() const
{
    return (_users.size() + _operators.size());
}

void Channel::add_user(const User& user)
{
    _users.push_back(user);
}

void Channel::remove_user(const User& user)
{
    _users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
}

void Channel::add_operator(const User& user)
{
    _operators.push_back(user);
}

void Channel::remove_operator(const User& user)
{
    _operators.erase(std::remove(_operators.begin(), _operators.end(), user), _operators.end());
}

std::string Channel::get_name() const
{
    return (_name);
}

std::string Channel::get_topic() const
{
    return (_topic);
}

void Channel::set_topic(const User& user, const std::string& topic)
{
    // Check if the user is an operator of the channel
    if (std::find(_operators.begin(), _operators.end(), user) == _operators.end())
        throw std::invalid_argument("User is not an operator of the channel");

    // If the user is an operator, set the topic
    _topic = topic;
}


