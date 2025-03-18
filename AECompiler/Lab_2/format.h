#pragma once
#include <vector>
#include <string>

std::vector<std::string> split(const std::string& str, const std::string& delimiter);

std::string format(const std::string& formatStr, const std::vector<std::string>& params);

std::string str_tolower(std::string s);