#include "utils.hpp"

std::string to_lower(std::string text)
{
	for (char &c : text)
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	return (text);
}

std::string to_upper(std::string text)
{	for (char &c : text)
		c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
	return (text);
}
