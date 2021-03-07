#ifndef PARSERMASSIF_H
#define PARSERMASSIF_H

#include <iostream>
#include <fstream>
#include <string>

class ParserMassif
{
public:
    ParserMassif();
    ParserMassif(std::string inputFileName);

    void parseMassifOutput();
    void parseMsPrintOutput();

private:
    std::string _inputFileName;
};

#endif // PARSERMASSIF_H
