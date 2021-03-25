#include "helper-functions.h"
#include <iostream>

std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);

    return start == end ? line : line.substr(start, end - start + 1);
}
