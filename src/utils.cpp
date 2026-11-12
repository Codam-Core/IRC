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

// a letter or special first, then letters, digits, specials or '-'
bool is_valid_nickname(const std::string &nickname)
{
	const std::string specials = "[]\\`_^{|}";

	if (nickname.empty() || nickname.size() > MAX_NICK_LENGTH)
		return (false);

	for (size_t i = 0; i < nickname.size(); i++)
	{
		char c = nickname[i];
		unsigned char uc = static_cast<unsigned char>(c);

		bool allowed = std::isalpha(uc) || specials.find(c) != std::string::npos;
		if (i > 0)
			allowed = allowed || std::isdigit(uc) || c == '-';

		if (!allowed)
			return (false);
	}
	return (true);
}
