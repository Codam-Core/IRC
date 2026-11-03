
#include "Message.hpp"

static bool skip_spaces(const std::string &line, size_t &pos)
{
	pos = line.find_first_not_of(' ', pos);
	if (pos == std::string::npos)
		return (false);
	else
		return (true);
}

static std::string next_word(const std::string &line, size_t &pos)
{
	size_t end = line.find(' ', pos);
	if (end == std::string::npos)
		end = line.length();

	std::string word = line.substr(pos, end - pos);
	pos = end;
	return (word);
}

static std::string to_upper_case(const std::string lc_command)
{
	std::string uc_command;
	for (size_t i = 0; i < lc_command.length(); i++)
	{
		uc_command[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(lc_command[i])));
	}
	return (uc_command);
}

std::optional<Message> parse_message(const std::string &line)
{
	Message	msg;
	size_t	pos = 0;

	if (!skip_spaces(line, pos))
		return (std::nullopt);

	if (line[pos] == ':')
	{
		pos++;
		msg.prefix = next_word(line, pos);
		if (!skip_spaces(line, pos))
			return (std::nullopt);
	}

	std::string command = next_word(line, pos);
	msg.command = to_upper_case(command);

	while (skip_spaces(line, pos))
	{
		if (line[pos] == ':')
		{
			msg.params.push_back(line.substr(pos + 1));
			break;
		}
		msg.params.push_back(next_word(line, pos));
	}

	return (msg);
}
