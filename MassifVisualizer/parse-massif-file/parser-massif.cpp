#include "parser-massif.h"

ParserMassif::ParserMassif(std::string inputFileName)
    : _inputFileName(inputFileName)
{

}

ParserMassif::~ParserMassif()
{
    for (SnapshotItem* si: _snapshotItems) {
        delete si;
    }
}

/*
    desc: --massif-out-file=izlaz_masif
    cmd: ./a.out
    time_unit: i
    #-----------
    snapshot=0
    #-----------
    time=0
    mem_heap_B=0
    mem_heap_extra_B=0
    mem_stacks_B=0
    heap_tree=empty/peak/detailed
*/

void ParserMassif::parseMassifOutput()
{
    std::string line;
    std::vector<std::string> lines;
    std::ifstream inputFile(_inputFileName);
    _snapshotItems.clear();

    if (inputFile.is_open()) {
        std::getline(inputFile, line);
        parseDescLine(line);

        if (!validMassifFile) {
            inputFile.close();
            return;
        }

        std::getline(inputFile, line);
        parseCmdLine(line);

        if (!validMassifFile) {
            inputFile.close();
            return;
        }

        std::getline(inputFile, line);
        parseTimeUnitLine(line);

        if (!validMassifFile) {
            inputFile.close();
            return;
        }

        // to swallow this: #-----------
        std::getline(inputFile, line);
        if (line.compare("#-----------")) {
            validMassifFile = false;
            return;
        }

        while (std::getline(inputFile, line) && validMassifFile) {
            if (!line.compare(0, 9, "snapshot=")) {
                lines.clear();

                SnapshotItem* newSnapshot = new SnapshotItem;

                // to extract snapshot=#
                uint snapshotNum = parseSnapshotNumberLine(line);
                newSnapshot->setSnapshotNum(snapshotNum);

                // to swallow this: #-----------
                std::getline(inputFile, line);
                if (line.compare("#-----------")) {
                    validMassifFile = false;
                    return;
                }

                // to extract time=#
                std::getline(inputFile, line);
                quint64 timeValue = parseTimeValueLine(line);
                newSnapshot->setTime(timeValue);

                // to extract mem_heap_B=#
                std::getline(inputFile, line);
                quint64 memHeapB = parseMemHeapBLine(line);
                newSnapshot->setMemHeapB(memHeapB);

                // to extract mem_heap_extra_B=#
                std::getline(inputFile, line);
                quint64 memHeapExtraB = parseMemHeapExtraBLine(line);
                newSnapshot->setMemHeapExtraB(memHeapExtraB);

                // to extract mem_stacks_B=#
                std::getline(inputFile, line);
                quint64 memStacksB = parseMemStacksBLine(line);
                newSnapshot->setMemStacksB(memStacksB);

                // to extract heap_tree
                std::getline(inputFile, line);
                if (!line.compare("heap_tree=empty")) {
                    // to swallow this: #-----------
                    std::getline(inputFile, line);
                    if (line.compare("#-----------") && !inputFile.eof()) {
                        validMassifFile = false;
                        return;
                    }
                }
                else if (!line.compare("heap_tree=detailed") || !line.compare("heap_tree=peak")){
                    while (line.compare("#-----------") && !inputFile.eof()) {
                        lines.emplace_back(line);
                        std::getline(inputFile, line);
                    }

                    std::pair<std::string, HeapTreeItem*> returnTypeAndHeapTree = parseHeapTreeLines(lines);

                    if (!returnTypeAndHeapTree.first.compare("detailed")) {
                        newSnapshot->setTreeType(HeapTreeType::DETAILED);
                    }
                    else {
                        newSnapshot->setTreeType(HeapTreeType::PEAK);
                        _peakSnapshot = newSnapshot;
                    }

                    newSnapshot->setHeapTreeItem(returnTypeAndHeapTree.second);
                }
                else {
                    //std::cout << "Nije empty, detailed ili peak!" << std::endl;
                    validMassifFile = false;
                    return;
                }
                _snapshotItems.push_back(newSnapshot);
            }
            else {
                //std::cout << "Nije snapshot!" << std::endl;
                validMassifFile = false;
                inputFile.close();
                return;
            }
        }
        inputFile.close();
    }
    else {
        //std::cout << "File is not opened." << std::endl;
    }
}

bool ParserMassif::validateMassifFile()
{
    return validMassifFile;
}

std::vector<SnapshotItem *> ParserMassif::snapshotItems() const
{
    return _snapshotItems;
}

std::string ParserMassif::exeFile() const
{
    return _exeFile;
}

std::string ParserMassif::timeUnit() const
{
    return _timeUnit;
}

SnapshotItem *ParserMassif::peakSnapshot() const
{
    return _peakSnapshot;
}

void ParserMassif::parseDescLine(const std::string &line)
{
    if (line.substr(0, 5).compare("desc:")) {
        validMassifFile = false;
        return;
    }

    //    std::string lineCopy("desc: --massif-out-line=andja --alloc-func=bla --traa=tralal");
    std::string delimiter = "--";
    std::vector<size_t> positions;

    // finding all -- positions in the line
    size_t pos = line.find(delimiter, 0);
    while(pos != std::string::npos)
    {
        positions.push_back(pos);
        pos = line.find(delimiter, pos+1);
    }

    // separating flags with arguments following positions
    std::vector<std::string> arguments;

    if (positions.size() == 0)
        return;

    for (size_t i=1; i < positions.size(); i++){
        arguments.push_back(line.substr(
                positions[i-1]+delimiter.size(),
                positions[i]-positions[i-1]-delimiter.size()));
    }
    arguments.push_back(line.substr(positions[positions.size()-1]+delimiter.size()));

    // parsing each flag and its value, separated by =
    delimiter = "=";
    for (size_t i=0; i<arguments.size(); i++){
        size_t equalPos = arguments.at(i).find(delimiter);
        std::string key = arguments.at(i).substr(0, equalPos);
        std::string value = trim(arguments.at(i).substr(equalPos+delimiter.size()));

        _descArgs[key] = trim(value);
    }
}

void ParserMassif::parseCmdLine(const std::string &line)
{
    if (line.substr(0, 4).compare("cmd:")) {
        validMassifFile = false;
        return;
    }

     std::string target = " ";
     size_t start = line.find_first_of(target);
     _exeFile = trim(line.substr(start+target.size()));
}

void ParserMassif::parseTimeUnitLine(const std::string &line)
{
    if (line.substr(0, 10).compare("time_unit:")) {
        validMassifFile = false;
        return;
    }
    std::string target = ":";
    std::string tmpTimeUnit = trim(line.substr(line.find(target)+target.size()+1));

    if (!tmpTimeUnit.compare("B") || !tmpTimeUnit.compare("ms") || !tmpTimeUnit.compare("i"))
        _timeUnit = tmpTimeUnit;
    else {
        validMassifFile = false;
        return;
    }
}

uint ParserMassif::parseSnapshotNumberLine(const std::string &line)
{
    std::string tmpString = trim(line.substr(line.find("=")+1));
    bool has_only_digits = tmpString.find_first_not_of("0123456789") == std::string::npos;
    if (line.size() > 9 && has_only_digits) {
        uint snapshotNum = static_cast<uint>(std::stoull(tmpString));
        return snapshotNum;
    }
    validMassifFile = false;
    return 0;
}

quint64 ParserMassif::parseTimeValueLine(const std::string &line)
{
    if (line.substr(0, 5).compare("time=")) {
        validMassifFile = false;
        return 0;
    }

    std::string tmpString = trim(line.substr(line.find("=")+1));
    bool has_only_digits = tmpString.find_first_not_of("0123456789") == std::string::npos;
    if (line.size() > 5 && has_only_digits) {
        quint64 timeValue = std::stoull(tmpString);
        return timeValue;
    }
    validMassifFile = false;
    return 0;
}

quint64 ParserMassif::parseMemHeapBLine(const std::string &line)
{
    if (line.substr(0, 11).compare("mem_heap_B=")) {
        validMassifFile = false;
        return 0;
    }

    std::string tmpString = trim(line.substr(line.find("=")+1));
    bool has_only_digits = tmpString.find_first_not_of("0123456789") == std::string::npos;
    if (line.size() > 11 && has_only_digits) {
        quint64 memHeapB = static_cast<quint64>(std::stoull(tmpString));
        return memHeapB;
    }

    validMassifFile = false;
    return 0;
}

quint64 ParserMassif::parseMemHeapExtraBLine(const std::string &line)
{
    if (line.substr(0, 17).compare("mem_heap_extra_B=")) {
        validMassifFile = false;
        return 0;
    }

    std::string tmpString = trim(line.substr(line.find("=")+1));
    bool has_only_digits = tmpString.find_first_not_of("0123456789") == std::string::npos;
    if (line.size() > 17 && has_only_digits) {
        quint64 memHeapExtraB = static_cast<quint64>(std::stoull(tmpString));
        return memHeapExtraB;
    }

    validMassifFile = false;
    return 0;
}

quint64 ParserMassif::parseMemStacksBLine(const std::string &line)
{
    if (line.substr(0, 13).compare("mem_stacks_B=")) {
        validMassifFile = false;
        return 0;
    }

    std::string tmpString = trim(line.substr(line.find("=")+1));
    bool has_only_digits = tmpString.find_first_not_of("0123456789") == std::string::npos;
    if (line.size() > 13 && has_only_digits) {
        quint64 memStacksB = static_cast<quint64>(std::stoull(tmpString));
        return memStacksB;
    }

    validMassifFile = false;
    return 0;
}

std::pair<std::string, HeapTreeItem*> ParserMassif::parseHeapTreeLines(std::vector<std::string> &lines)
{
    HeapTreeItem* newHeapTree = new HeapTreeItem;
    newHeapTree->setIndentation(0);
    std::pair<std::string, HeapTreeItem*> returnTypeAndHeapTree;

    std::string target = "=";
    std::string treeType = lines[0].substr(lines[0].find(target)+target.size());
    returnTypeAndHeapTree.first = treeType;

    lines.erase(lines.begin());

    //n3: 10000 (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
    std::string line = lines[0];
    size_t start = line.find("n");
    size_t end = line.find(":");

    uint numOfDirectChildren = static_cast<uint>(checkIfNumber(line, start+1, end-start-1));
    newHeapTree->setNumOfDirectChildren(numOfDirectChildren);

    start = line.find(" ", end+1);
    end = line.find(" ", start+1);

    quint64 memoryAlloc = checkIfNumber(line, start+1, end-start-1);

    newHeapTree->setMemoryAlloc(memoryAlloc);
    newHeapTree->setMother(nullptr); // the first line is our GRoot :)
    lines.erase(lines.begin());

    auto currentMother = newHeapTree;
    for (auto line : lines) {
        if (line.find("threshold") != std::string::npos)
            continue;
        HeapTreeItem* newTree = new HeapTreeItem;

        size_t posN = line.find("n");
        size_t posTwoDots = line.find(":");

        uint numOfDirectChildren = static_cast<uint>(checkIfNumber(line, posN+1, posTwoDots-posN-1));

        newTree->setNumOfDirectChildren(numOfDirectChildren);

        size_t start = line.find(" ", posTwoDots);
        size_t end = line.find(" ", start+1);
        quint64 memoryAlloc = checkIfNumber(line, start+1, end-start-1);
        newTree->setMemoryAlloc(memoryAlloc);

        // Cases:
        // 1. normal: n0: 10000 0x10917D: main (basic.c:9)
        // 2. without -g compile: n2: 8000 0x109161: g (in /home/student/Desktop/massif)
        // 3. function name c++: n1: 21949 0x6125403: KDebug::hasNullOutput(QtMsgType, bool, int, bool) (kdebug.cpp:773)
        // 4. stacks = yes: n3: 10000 (heap allocation functions) malloc/new/new[], --alloc-fns, etc.

        start = end;
        end = line.find(":", start+1);
        std::string memoryAddr = trim(line.substr(start+1, end-start-1));
        newTree->setMemoryAddr(memoryAddr);

        start = line.find(" ", end+1);
        end = line.find_last_of("(");
        std::string funcName = trim(line.substr(start+1, end-start-1-1)); // one more -1 because of the space
        newTree->setFuncName(funcName);

        uint lineNum = 0;
        std::string fileName;
        if (line.find("(in ") != std::string::npos) {
            start = line.find("(in ") + 3;
            end = line.find_last_of(")");
            fileName = trim(line.substr(start+1, end-start-1));
        }
        else {
            start = end;
            end = line.find(":", start+1);
            fileName = trim(line.substr(start+1, end-start-1));

            start = end;
            end = line.find(")", start+1);
            lineNum = static_cast<uint>(checkIfNumber(line, start+1, end-start-1));
        }
        newTree->setFileName(fileName);
        newTree->setLineNum(lineNum);

        while(currentMother->children().size() == currentMother->numOfDirectChildren() && validMassifFile){
            currentMother = currentMother->mother();
        }

        currentMother->addChild(newTree);
        newTree->setIndentation(currentMother->indentation() + 1);

        if(numOfDirectChildren != 0) {
            currentMother = newTree;
        }
    }
    returnTypeAndHeapTree.second = newHeapTree;

    if (validMassifFile)
        return returnTypeAndHeapTree;
    else {
        return std::pair("", nullptr);
    }
}

std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);

    return (start==std::string::npos && start==end) ? line : line.substr(start, end - start + 1);
}

quint64 ParserMassif::checkIfNumber(const std::string &line, size_t start, size_t end)
{
    std::string tmpString = trim(line.substr(start, end));
    bool has_only_digits = tmpString.find_first_not_of("0123456789") == std::string::npos;

    if (has_only_digits && tmpString.size() > 0) {
        return std::stoull(tmpString);
    }
    else {
        validMassifFile = false;
        return 0;
    }
}
