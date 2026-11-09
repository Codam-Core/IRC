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

// "#a,#b,#c" → ["#a", "#b", "#c"]; empty parts (like in "#a,,#b") are skipped
std::vector<std::string> split(const std::string &text, char delimiter)
{
	std::vector<std::string> parts;
	std::stringstream ss(text);
	std::string part;

	while (std::getline(ss, part, delimiter))
	{
		if (!part.empty())
			parts.push_back(part);
	}
	return (parts);
}
