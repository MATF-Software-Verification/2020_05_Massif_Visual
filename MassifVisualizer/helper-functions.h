#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <QDialog>
enum class Theme {
    DEFAULT,
    BRIGHT,
    PSYCHEDELIC,
    SAPPHIRE
};

//GASP a global variable!? HOW COULD THEY!!! *clutches pearls
extern Theme theme;

std::string trim(const std::string& line);
void setTheme(QDialog* dialog);


#endif // HELPERFUNCTIONS_H
