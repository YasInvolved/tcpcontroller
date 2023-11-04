#include <vector>
#include <string>
#pragma once

std::vector<std::string> split(std::string str, std::string separator)
{
    std::vector<std::string> tokens;
    size_t pos;
    while ((pos = str.find(separator)) != std::string::npos)
    {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + separator.length());
    }
    tokens.push_back(str);
    return tokens;
}