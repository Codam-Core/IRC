#pragma once

#include "includes.hpp"

struct Message
{
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	params;
};

std::optional<Message> parse_message(const std::string &line);