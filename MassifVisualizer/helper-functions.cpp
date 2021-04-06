#include "helper-functions.h"
#include <iostream>

std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);

    return start == end ? line : line.substr(start, end - start + 1);
}

void setTheme(QWidget* widget)
{
    QString labelStyle = "";
    QString lineEditStyle = "";
    QString backgroundStyle = "";
    QString textStyle = "";
    QString btnStyle = "";
    QString btnPressed = "";
    QString btnDisabled = "";

    switch(theme){
        case Theme::DEFAULT:
            labelStyle = "QLabel {color: #ff8000; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #262626}\n";
            backgroundStyle = "* {background-color: #4d4d4d}\n";
            textStyle = "* {color : white}\n";
            btnStyle = "QPushButton { background-color: #ff8000; border: 1px solid #ff8000; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}\n";
            btnDisabled = "QPushButton:disabled { background-color: #ffe0b3; border: 1px solid black;}";
            widget->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed + btnDisabled);
            break;
        case Theme::BRIGHT:
            labelStyle = "QLabel {color: black; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #ffe6cc}\n";
            backgroundStyle = "* {background-color: #f2f2f2}\n";
            textStyle = "* {color : black}\n";
            btnStyle = "QPushButton { background-color: #ffe6cc; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}\n";
            btnDisabled = "QPushButton:disabled { background-color: white; border: 1px solid black;}";
            widget->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed + btnDisabled);
            break;
        case Theme::PSYCHEDELIC:
            psychedelicTheme(widget);
            break;
        case Theme::SAPPHIRE:
            labelStyle = "QLabel {color: #e6f3ff; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #cce6ff; color: #004080}\n";
            backgroundStyle = "* {background-color: #004080}\n";
            textStyle = "* {color : #e6f3ff}\n";
            btnStyle = "QPushButton { background-color: #4db8ff; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color:  #ccebff; border: 1px solid black;}\n";
            btnDisabled = "QPushButton:disabled { background-color: #cce6ff; border: 1px solid black;}";
            widget->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed + btnDisabled);
            break;
    }
}

void psychedelicTheme(QWidget *widget)
{
    QString backgroundRgba;
    QString textColorRgba;
    generateRandomColors(backgroundRgba, textColorRgba);

    QString LERgba;
    QString LETextRgba;
    generateRandomColors(LERgba, LETextRgba);

    QString btnRgba;
    QString btnTextRgba;
    generateRandomColors(btnRgba, btnTextRgba);

    QString btnPressedRgba;
    QString btnPressedTextRgba;
    generateRandomColors(btnPressedRgba, btnPressedTextRgba);

    QString btnDisabledRgba;
    QString btnDisabledTextRgba;
    generateRandomColors(btnDisabledRgba, btnDisabledTextRgba);

    QString btnStyle = "QPushButton { background-color: " + btnRgba + "; color: " + btnTextRgba + "; padding: 3px; font-size: 12pt; font-weight: bold}\n";
    QString btnPressedStyle = "QPushButton:pressed { background-color: " + btnPressedRgba + "; color: " + btnPressedTextRgba + "}\n";
    QString btnDisabledStyle = "QPushButton:disabled { background-color: " + btnDisabledRgba + "; color: " + btnDisabledTextRgba + ";}\n";

    QString backgroundOfElements = "* {background-color: " + backgroundRgba + "}\n";
    QString textColor = "* {color: " + textColorRgba + "; font-weight: bold}\n";

    QString lineEditStyle = "QLineEdit {background-color: " + LERgba + "; color: " + LETextRgba + "}\n";
    QString labelStyle = "QLabel {font-size: 12pt; font-weight: bold}";

    widget->setStyleSheet(btnStyle + btnPressedStyle + btnDisabledStyle + backgroundOfElements + textColor + lineEditStyle + labelStyle);
}

void generateRandomColors(QString &backgroundColor, QString &textColor)
{
    int backgroundRed = QRandomGenerator::global()->bounded(255);
    int backgroundGreen = QRandomGenerator::global()->bounded(255);
    int backgroundBlue = QRandomGenerator::global()->bounded(255);
    backgroundColor = "rgba(" + QString::number(backgroundRed) + "," + QString::number(backgroundGreen)  + "," + QString::number(backgroundBlue) + ", 1)";
    int textColorRed = 255-backgroundRed;
    int textColorGreen = 255-backgroundGreen;
    int textColorBlue = 255-backgroundBlue;
    textColor = "rgba(" + QString::number(textColorRed) + "," + QString::number(textColorGreen)  + "," + QString::number(textColorBlue) + ", 255)";
}
