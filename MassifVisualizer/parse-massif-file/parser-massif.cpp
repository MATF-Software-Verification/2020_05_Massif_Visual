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
