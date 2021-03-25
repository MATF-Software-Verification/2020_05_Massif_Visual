#include "parser-massif.h"
#include "helper-functions.h"

ParserMassif::ParserMassif()
{
    _inputFileName = ("../MassifVisualizer/input-examples/massif_output1");
}

ParserMassif::ParserMassif(std::string inputFileName)
    : _inputFileName(inputFileName)
{

}

ParserMassif::~ParserMassif()
{
    for (SnapshotItem* si: _snapshotItems) {
        delete si;
    }
    // TODO: napisati obican c++ program van Qt i pokrenuti Valgrind --massif
    // u kom ce se meriti da li se poziva default destruktor za mapu
    // default destructor for a map containing primitive types will free memory
    // _descArgs
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

    if (inputFile.is_open()) {
        std::getline(inputFile, line);
        parseDescLine(line);
        std::getline(inputFile, line);
        parseCmdLine(line);
        std::getline(inputFile, line);
        parseTimeUnitLine(line);

        // to swallow this: #-----------
        std::getline(inputFile, line);

        while (std::getline(inputFile, line)) {
            //std::cout << line << '\n';
            if (!line.compare(0, 8, "snapshot")) {
                lines.clear();

                SnapshotItem* newSnapshot = new SnapshotItem;

                // to extract snapshot=#
                uint snapshotNum = parseSnapshotNumberLine(line);
                newSnapshot->setSnapshotNum(snapshotNum);

                // to swallow this: #-----------
                std::getline(inputFile, line);

                // to extract time=#
                std::getline(inputFile, line);
                double timeValue = parseTimeValueLine(line);
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
                newSnapshot->setMemHeapB(memStacksB);

                // to extract heap_tree
                std::getline(inputFile, line);
                if (!line.compare("heap_tree=empty")) {
                    std::cout << "Heap tree is empty here." << std::endl;
                    // to swallow this: #-----------
                    std::getline(inputFile, line);
                }
                else {
                    std::cout << "Heap tree is not empty here." << std::endl;
                    std::cout << line << std::endl;

                    /*
                     * push_back("foo") constructs a temporary string from the string literal,
                     * and then moves that string into the container, whereas emplace_back("foo")
                     * just constructs the string directly in the container,
                     * avoiding the extra move.
                    */
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
                    }

                    newSnapshot->setHeapTreeItem(returnTypeAndHeapTree.second);
                }
                _snapshotItems.push_back(newSnapshot);
            }
        }
        inputFile.close();
    }
    else {
        std::cout << "File is not opened." << std::endl;
    }
}

void ParserMassif::parseMsPrintOutput()
{
    std::string line;
    std::ifstream inputFile(_inputFileName);
    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) {
            std::cout << line << '\n';
        }
    }
    else {
        std::cout << "File is not opened." << std::endl;
    }
}

std::vector<SnapshotItem *> ParserMassif::snapshotItems() const
{
    return _snapshotItems;
}

void ParserMassif::parseDescLine(const std::string &line)
{
    std::cout << "Desc line: " << line << std::endl;
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
    for (size_t i=1; i<positions.size(); i++){
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
     std::cout << "Cmd line: " << line << std::endl;
     std::string target = "./";
     size_t start = line.find_first_of(target);
     _exeFile = trim(line.substr(start+target.size()));
}

void ParserMassif::parseTimeUnitLine(const std::string &line)
{
    std::cout << "Time unit line: " << line << std::endl;
    std::string target = ":";
    _timeUnit = line.substr(line.find(target)+target.size());
}

uint ParserMassif::parseSnapshotNumberLine(const std::string &line)
{
    std::cout << "Snapshot line: " << line << std::endl;
    uint snapshotNum = static_cast<uint>(std::stoi(trim(line.substr(line.find("=")+1))));
    return snapshotNum;
}

double ParserMassif::parseTimeValueLine(const std::string &line)
{
    std::cout << "Time Value line: " << line << std::endl;
    double timeValue = std::stod(trim(line.substr(line.find("=")+1)));
    return timeValue;
}

quint64 ParserMassif::parseMemHeapBLine(const std::string &line)
{
    std::cout << "Mem heap B line: " << line << std::endl;
    quint64 memHeapB = static_cast<quint64>(std::stoi(trim(line.substr(line.find("=")+1))));
    return memHeapB;
}

quint64 ParserMassif::parseMemHeapExtraBLine(const std::string &line)
{
    std::cout << "Mem heap extra B line: " << line << std::endl;
    quint64 memHeapExtraB = static_cast<quint64>(std::stoi(trim(line.substr(line.find("=")+1))));
    return memHeapExtraB;
}

quint64 ParserMassif::parseMemStacksBLine(const std::string &line)
{
    std::cout << "Mem stacks B line: " << line << std::endl;
    quint64 memStacksB = static_cast<quint64>(std::stoi(trim(line.substr(line.find("=")+1))));
    return memStacksB;
}

std::pair<std::string, HeapTreeItem*> ParserMassif::parseHeapTreeLines(std::vector<std::string> &lines)
{
    HeapTreeItem* newHeapTree = new HeapTreeItem;
    std::pair<std::string, HeapTreeItem*> returnTypeAndHeapTree;

    std::string target = "=";
    std::string treeType = lines[0].substr(lines[0].find(target)+target.size());
    returnTypeAndHeapTree.first = treeType;

    lines.erase(lines.begin());

    //n3: 10000 (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
    std::string line = lines[0];
    size_t start = line.find("n");
    size_t end = line.find(":");
    uint numOfDirectChildren = static_cast<uint>(std::stoi(line.substr(start+1, end-start-1)));
    newHeapTree->setNumOfDirectChildren(numOfDirectChildren);

    start = line.find(" ", end+1);
    end = line.find(" ", start+1);
    quint64 memoryAlloc = static_cast<quint64>(std::stoi(trim(line.substr(start+1, end-start-1))));
    newHeapTree->setMemoryAlloc(memoryAlloc);
    newHeapTree->setMother(nullptr); // the first line is our GRoot :)
    lines.erase(lines.begin());

    auto currentMother = newHeapTree;
    for (auto line : lines) {
        HeapTreeItem* newTree = new HeapTreeItem;

        size_t posN = line.find("n");
        size_t posTwoDots = line.find(":");
        uint numOfDirectChildren = static_cast<uint>(std::stoi(line.substr(posN+1, posTwoDots-posN-1)));
        newTree->setNumOfDirectChildren(numOfDirectChildren);

        size_t start = line.find(" ", posTwoDots);
        size_t end = line.find(" ", start+1);
        quint64 memoryAlloc = static_cast<quint64>(std::stoi(trim(line.substr(start+1, end-start-1))));
        newTree->setMemoryAlloc(memoryAlloc);

        start = end;
        end = line.find(":", start+1);
        if (end != std::string::npos) {
            std::string memoryAddr = trim(line.substr(start+1, end-start-1));
            newTree->setMemoryAddr(memoryAddr);

            start = line.find(" ", end+1);
            end = line.find(" ", start+1);
            std::string funcName = trim(line.substr(start+1, end-start-1));
            newTree->setFuncName(funcName);

            start = line.find("(", end+1);
            end = line.find(":", start+1);
            std::string fileName = trim(line.substr(start+1, end-start-1));
            newTree->setFileName(fileName);

            start = end;
            end = line.find(")", start+1);
            uint lineNum = static_cast<uint>(std::stoi(trim(line.substr(start+1, end-start-1))));
            newTree->setLineNum(lineNum);
        }
        while(currentMother->children().size() == currentMother->numOfDirectChildren()){
            currentMother = currentMother->mother();
        }
        currentMother->addChild(newTree);

        if(numOfDirectChildren != 0) {
            currentMother = newTree;
        }
    }

    returnTypeAndHeapTree.second = newHeapTree;
    return returnTypeAndHeapTree;
}
