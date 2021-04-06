#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <QDialog>
#include <QRandomGenerator>

enum class Theme {
    DEFAULT,
    BRIGHT,
    PSYCHEDELIC,
    SAPPHIRE
};

//GASP a global variable!? HOW COULD THEY!!! *clutches pearls
extern Theme theme;

std::string trim(const std::string& line);
void setTheme(QWidget* widget);
void psychedelicTheme(QWidget* widget);
void generateRandomColors(QString& backgroundColor, QString& textColor);

#endif // HELPERFUNCTIONS_H
