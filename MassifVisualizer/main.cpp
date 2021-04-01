#include "mainwindow.h"
#include <QApplication>

#include "./parse-massif-file/parser-massif.h"
#include <regex>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    TODO: figure out how to execute valgrind from c++
//    system("valgrind --tool=massif --massif-out-file=opala /home/student/Desktop/a.out 2>&0");

//    std::cout << "OUTPUT=$(valgrind --tool=massif ./a.out 2>&1) | echo $OUTPUT | grep -o ==[0-9][0-9]* | head -1 | cut -c3-";
//    std::regex rgx("==([0-9]+)==");
//    std::smatch match;
//    if (std::regex_search(terminalOutputValgrind.begin(), s.end(), match, rgx))
//        std::cout << "match: " << match[1] << '\n';
//    std::cout << system("ls -ct -1 | grep \"massif\\.out\\.\" | head -1") << std::endl;
    return a.exec();
}
