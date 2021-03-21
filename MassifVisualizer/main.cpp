#include "mainwindow.h"
#include <QApplication>

#include "./parse-massif-file/parser-massif.h"
#include <regex>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    HeapTreeItem* ljuba = new HeapTreeItem();
    ljuba->setNumOfDirectChildren(3);
    ljuba->setMemoryAlloc(20000);

    HeapTreeItem* beka = new HeapTreeItem();
    beka->setNumOfDirectChildren(0);
    beka->setMemoryAlloc(10000);
    ljuba->addChild(beka);

    HeapTreeItem* ivana = new HeapTreeItem();
    ivana->setNumOfDirectChildren(2);
    ivana->setMemoryAlloc(8000);
    ljuba->addChild(ivana);

    HeapTreeItem* vuki = new HeapTreeItem();
    vuki->setNumOfDirectChildren(1);
    vuki->setMemoryAlloc(4000);
    ivana->addChild(vuki);

    HeapTreeItem* uki = new HeapTreeItem();
    uki->setNumOfDirectChildren(0);
    uki->setMemoryAlloc(4000);
    ivana->addChild(uki);

    HeapTreeItem* vukijevoDete = new HeapTreeItem();
    vukijevoDete->setNumOfDirectChildren(0);
    vukijevoDete->setMemoryAlloc(4000);
    vuki->addChild(vukijevoDete);

    HeapTreeItem* nikola = new HeapTreeItem;
    nikola->setNumOfDirectChildren(1);
    nikola->setMemoryAlloc(2000);
    ljuba->addChild(nikola);

    HeapTreeItem* sasa = new HeapTreeItem;
    sasa->setNumOfDirectChildren(0);
    sasa->setMemoryAlloc(2000);
    nikola->addChild(sasa);

//    std::cout << ljuba->heapTreeSum() << std::endl;

    // TODO: create unit testing using QtTest or Google Test
    // For now, this is testing
    ParserMassif parser = ParserMassif();
    parser.parseMassifOutput();

    delete ljuba;

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
