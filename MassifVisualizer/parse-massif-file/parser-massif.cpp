#include "parser-massif.h"

ParserMassif::ParserMassif()
{
    _inputFileName = ("../MassifVisualizer/input-examples/massif_output1");
}

ParserMassif::ParserMassif(std::string inputFileName)
    : _inputFileName(inputFileName)
{

}

void ParserMassif::parseMassifOutput()
{

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

void ParserMassif::parseDescLine(const std::string &line)
{
    std::cout << "Desc line: " << line << std::endl;
}

void ParserMassif::parseCmdLine(const std::string &line)
{
     std::cout << "Cmd line: " << line << std::endl;
}

void ParserMassif::parseTimeUnitLine(const std::string &line)
{
    std::cout << "Time unit line: " << line << std::endl;
}

void ParserMassif::parseSnapshotNumberLine(const std::string &line)
{
    std::cout << "Snapshot line: " << line << std::endl;
}

void ParserMassif::parseTimeValueLine(const std::string &line)
{
    std::cout << "Time Value line: " << line << std::endl;
}

void ParserMassif::parseMemHeapBLine(const std::string &line)
{

}
