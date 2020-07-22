#include "Utils.h"
#include <iostream>
#include <algorithm>

std::string Utils::uppercase(std::string t_str) {
    std::transform(t_str.begin(), t_str.end(), t_str.begin(),
        [&](unsigned char c) {return std::toupper(c);});
    
    return t_str;
};