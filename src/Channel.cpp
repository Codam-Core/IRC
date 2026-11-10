#include "Channel.hpp"
#include "colours.hpp"
#include "debug.hpp"

// constructor/desructor
Channel::Channel(const std::string& name) : _name(name)
{
    debug(colour::YELLOW, "Channel", "constructor called: channel " + _name + " created");
}

Channel::~Channel(void)
{
    debug(colour::YELLOW, "Channel", "destructor called: channel " + _name + " destroyed");
}


// members
void Channel::add_member(Client *client, bool is_operator)
{
	_members[client] = is_operator;
}

void Channel::remove_member(Client *client)
{
	_members.erase(client);
}

bool Channel::is_member(Client *client) const
{
	return (_members.find(client) != _members.end()); // find looks for the key. If it isn't there, you get end().
}

bool Channel::is_operator(Client *client) const
{
	auto it = _members.find(client);
	if (it == _members.end())
		return (false);         // not a member at all → certainly not an operator
	return (it->second);        // the bool: operator here or not
}

void Channel::set_operator(Client *client, bool is_operator)
{
	auto it = _members.find(client);
	if (it == _members.end())
		return ; 
	it->second = is_operator;
} 

size_t Channel::member_count() const
{
	return (_members.size());
}

const std::map<Client *, bool>& Channel::get_members() const
{
	return (_members);
} 


// invites
void Channel::add_invite(Client *client)
{
	_invited.insert(client);
}

void Channel::remove_invite(Client *client)
{
	_invited.erase(client);
}

bool Channel::is_invited(Client *client) const
{
	if (_invited.find(client) != _invited.end())
		return (true);
	else
		return (false);
}


// key and limit
const std::string& Channel::get_key() const
{
	return (_key);
}

void Channel::set_key(const std::string &key)
{
	_key = key;
}

size_t Channel::get_limit() const
{
	return (_limit);
}

void Channel::set_limit(size_t limit)
{
	_limit = limit;
}


// modes
bool Channel::has_mode(modes mode) const
{
    return _mode & mode; // AND
}

void Channel::set_mode(modes mode)
{
    _mode |= mode; // OR
}

void Channel::unset_mode(modes mode)
{
    _mode &= ~mode; // AND NOT
}


// getters and setters
std::string Channel::get_name() const
{
    return (_name);
}

std::string Channel::get_topic() const
{
    return (_topic);
}

void Channel::set_topic(const std::string &topic)
{
    _topic = topic;
}


