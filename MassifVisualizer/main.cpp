#include "mainwindow.h"
#include <QApplication>

#include "./parse-massif-file/parser-massif.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // TODO: create unit testing using QtTest or Google Test
    // For now, this is testing
    ParserMassif parser = ParserMassif();
    parser.parseMassifOutput();

    return a.exec();
}
