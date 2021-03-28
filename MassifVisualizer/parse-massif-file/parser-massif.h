#ifndef PARSERMASSIF_H
#define PARSERMASSIF_H

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "./snapshotitem.h"

class ParserMassif
{
public:
    ParserMassif();
    ParserMassif(std::string inputFileName);
    ~ParserMassif();

    void parseMassifOutput();
    void parseMsPrintOutput();

    std::vector<SnapshotItem *> snapshotItems() const;

    std::string exeFile() const;
    std::string timeUnit() const;

private:
    std::string _inputFileName;
    std::map<std::string, std::string> _descArgs;
    /*
        insert elements in random order
        _descArgs.insert(pair<str, str>("bla", "blah"));
    */
    std::string _exeFile;
    std::string _timeUnit;

    std::vector<SnapshotItem*> _snapshotItems;

    void parseDescLine(const std::string &line);
    void parseCmdLine(const std::string &line);
    void parseTimeUnitLine(const std::string &line);

    uint parseSnapshotNumberLine(const std::string &line);
    double parseTimeValueLine(const std::string &line);
    quint64 parseMemHeapBLine(const std::string &line);
    quint64 parseMemHeapExtraBLine(const std::string &line);
    quint64 parseMemStacksBLine(const std::string &line);
    std::pair<std::string, HeapTreeItem*> parseHeapTreeLines(std::vector<std::string> &lines);
};

#endif // PARSERMASSIF_H
