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
    ParserMassif(std::string inputFileName);
    ~ParserMassif();

    void parseMassifOutput();
    bool validateMassifFile();

    std::vector<SnapshotItem *> snapshotItems() const;

    std::string exeFile() const;
    std::string timeUnit() const;

    SnapshotItem *peakSnapshot() const;

private:
    std::string _inputFileName;
    std::map<std::string, std::string> _descArgs;
    std::string _exeFile;
    std::string _timeUnit;
    std::vector<SnapshotItem*> _snapshotItems;
    SnapshotItem* _peakSnapshot = nullptr;

    void parseDescLine(const std::string &line);
    void parseCmdLine(const std::string &line);
    void parseTimeUnitLine(const std::string &line);
    quint64 checkIfNumber(const std::string &line, size_t start, size_t end);

    uint parseSnapshotNumberLine(const std::string &line);
    quint64 parseTimeValueLine(const std::string &line);
    quint64 parseMemHeapBLine(const std::string &line);
    quint64 parseMemHeapExtraBLine(const std::string &line);
    quint64 parseMemStacksBLine(const std::string &line);
    std::pair<std::string, HeapTreeItem*> parseHeapTreeLines(std::vector<std::string> &lines);

    bool validMassifFile = true;
};

std::string trim(const std::string& line);

#endif // PARSERMASSIF_H
