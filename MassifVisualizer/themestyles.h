#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <QDialog>
#include <QRandomGenerator>

enum class Theme {
    DEFAULT,
    BRIGHT,
    PSYCHEDELIC,
    SAPPHIRE,
    CORN
};

//GASP a global variable!? HOW COULD THEY!!! *clutches pearls
extern Theme theme;

void setTheme(QWidget* widget);
void psychedelicTheme(QWidget* widget);
void generateRandomColors(QString& backgroundColor, QString& textColor);

#endif // HELPERFUNCTIONS_H
