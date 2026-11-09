#pragma once

#include "includes.hpp"

inline constexpr size_t MAX_NICK_LENGTH = 9; // RFC 2812

std::string to_lower(std::string text);
std::string to_upper(std::string text);
std::vector<std::string> split(const std::string &text, char delimiter);
bool is_valid_nickname(const std::string &nickname);
