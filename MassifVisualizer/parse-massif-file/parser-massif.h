#ifndef PARSERMASSIF_H
#define PARSERMASSIF_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "./snapshotitem.h"

class ParserMassif
{
public:
    ParserMassif();
    ParserMassif(std::string inputFileName);

    void parseMassifOutput();
    void parseMsPrintOutput();

private:
    std::string _inputFileName;
    std::map<std::string, std::string> _descArgs;
    /*
        insert elements in random order
        _descArgs.insert(pair<str, str>("bla", "blah"));
    */
    std::string _exeFile;
    std::string _timeUnit;

    std::vector<SnapshotItem*> snapshotItems;

    void parseDescLine(const std::string &line);
    void parseCmdLine(const std::string &line);
    void parseTimeUnitLine(const std::string &line);

    void parseSnapshotNumberLine(const std::string &line);
    void parseTimeValueLine(const std::string &line);
    void parseMemHeapBLine(const std::string &line);
    void parseMemHeapExtraBLine(const std::string &line);
    void parseMemStacksBLine(const std::string &line);
    void parseHeapTreeLines(std::vector<std::string> &lines);
};

#endif // PARSERMASSIF_H
