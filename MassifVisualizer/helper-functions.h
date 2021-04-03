#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>

enum class Theme {
    DEFAULT,
    BRIGHT,
    PSYCHEDELIC,
    SAPPHIRE
};

//GASP a global variable!? HOW COULD THEY!!! *clutches pearls
extern Theme theme;

std::string trim(const std::string& line);

#endif // HELPERFUNCTIONS_H
