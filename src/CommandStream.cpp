#include "CommandStream.hpp"
#include "Server.hpp"

CommandStream::CommandStream(const std::string& command, Server &server): _command(command), _server(server)
{
}

CommandStream::~CommandStream()
{
}

CommandStream::CommandStream(const CommandStream& other): std::basic_ios<char>(), std::stringstream(other.str()), _responses(other._responses), _command(other._command), _server(other._server)
{
}

CommandStream CommandStream::operator=(const CommandStream& other)
{
    if (this != &other)
    {
        this->_command = other._command;
        this->_responses = other._responses;
    }
    return *this;
}

CommandStream& CommandStream::prefix()
{
    *this << ":" << "irc.JL" << " ";
    return *this;
}

CommandStream& CommandStream::prefix(const Client& client)
{
    *this << ":" << client.get_nickname() << "!" << client.get_username() << "@" << client.get_hostname() << " ";
    return *this;
}

CommandStream& CommandStream::prefix(Client& client, Channel& channel)
{
    (void)client; // unused parameter
    (void)channel; // unused parameter
    // Implement this later when channel operators are implemented
    *this << "Not implemented yet";
    return *this;
}

CommandStream& CommandStream::code(int code)
{
    *this << std::setw(3) << std::setfill('0') << code << " ";
    return *this;
}

CommandStream& CommandStream::client(const Client& client)
{
    if (client.get_nickname().empty())
        *this << "*" << " ";
    else
        *this << client.get_nickname() << " ";
    return *this;
}

CommandStream& CommandStream::parameter(const std::string& param)
{
    *this << param << " ";
    return *this;
}

CommandStream& CommandStream::parameter(const std::vector<std::string>& params)
{
    for (auto &param : params)
    {
        *this << param << " ";
    }
    return *this;
}

CommandStream& CommandStream::message(const std::string& msg)
{
    *this << ":" << msg;
    return *this;
}

CommandStream& CommandStream::command()
{
    *this << _command << " ";
    return *this;
}

CommandStream& CommandStream::get_member_names(const Channel& channel)
{
    // like message(): the list is the last parameter, so it starts with ':'
    *this << ":";

    bool first = true;
    for (auto &[member, is_operator] : channel.get_members())
    {
        if (!first)
            *this << " ";
        first = false;

        if (is_operator)
            *this << "@";
        *this << member->get_nickname();
    }
    return *this;
}

void CommandStream::finish()
{
    // if the stringstream is empty, don't add a CRLF and don't push it to the responses vector
    if (this->str().empty())
        return;

    // add the CRLF to the end of the stringstream, and push it to the responses vector
    _responses.push_back(this->str() + "\r\n");

    // clear the stringstream for the next command
    this->str("");
}

void CommandStream::send(Client& client)
{
    for (auto &response : _responses)
    {
        _server.send_to_client(client, response); 
    }
}

// send to all members of a channel. optional: to skip the sender with except
void CommandStream::send_to_channel(Channel &channel, Client *except)
{
    for (auto &member : channel.get_members())
	{
		if (member.first == except)
			continue;

		for (auto &response : _responses)
		{
			_server.send_to_client(*member.first, response);
		}
	}
}
