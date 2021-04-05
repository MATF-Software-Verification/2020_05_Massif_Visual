#include "helper-functions.h"
#include <iostream>

std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);

    return start == end ? line : line.substr(start, end - start + 1);
}

void setTheme(QDialog* dialog)
{
    QString labelStyle = "";
    QString lineEditStyle = "";
    QString backgroundStyle = "";
    QString textStyle = "";
    QString btnStyle = "";
    QString btnPressed = "";
    switch(theme){
        case Theme::DEFAULT:
            labelStyle = "QLabel {color: #ff8000; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #262626}\n";
            backgroundStyle = "* {background-color: #4d4d4d}\n";
            textStyle = "* {color : white}\n";
            btnStyle = "QPushButton { background-color: #ff8000; border: 1px solid #ff8000; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}";
            dialog->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
        case Theme::BRIGHT:
            labelStyle = "QLabel {color: black; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #ffe6cc}\n";
            backgroundStyle = "* {background-color: #f2f2f2}\n";
            textStyle = "* {color : black}\n";
            btnStyle = "QPushButton { background-color: #ffe6cc; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}";
            dialog->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
        case Theme::PSYCHEDELIC:
            labelStyle = "QLabel {color: #0033cc; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #993399}\n";
            backgroundStyle = "* {background-color: #4dffa6}\n";
            textStyle = "* {color : #26004d}\n";
            btnStyle = "QPushButton { background-color: #ffe6cc; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}";
            dialog->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
        case Theme::SAPPHIRE:
            labelStyle = "QLabel {color: #e6f3ff; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #cce6ff; color: #004080}\n";
            backgroundStyle = "* {background-color: #004080}\n";
            textStyle = "* {color : #e6f3ff}\n";
            btnStyle = "QPushButton { background-color: #4db8ff; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color:  #ccebff; border: 1px solid black;}";
            dialog->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
    }
}
