#include "mainwindow.h"
#include <QApplication>

#include "./parse-massif-file/parser-massif.h"
#include <regex>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
