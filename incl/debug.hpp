#pragma once

#include "includes.hpp"
#include "colours.hpp"

// set to false to turn off ALL debug output (for the evaluation)
inline constexpr bool DEBUG = true;

// one debug line: "[tag] text", with the tag in colour
inline void debug(const char *tag_colour, const std::string &tag, const std::string &text)
{
	if (DEBUG)
		std::cerr << tag_colour << "[" << tag << "] " << colour::RESET << text << std::endl;
}
