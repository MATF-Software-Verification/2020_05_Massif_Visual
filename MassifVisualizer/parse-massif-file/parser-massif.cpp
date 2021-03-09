#include "parser-massif.h"

ParserMassif::ParserMassif()
{
    _inputFileName = ("../MassifVisualizer/input-examples/massif_output1");
}

ParserMassif::ParserMassif(std::string inputFileName)
    : _inputFileName(inputFileName)
{

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

}

void ParserMassif::parseMsPrintOutput()
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
                // to extract snapshot=#
                parseSnapshotNumberLine(line);
                // to swallow this: #-----------
                std::getline(inputFile, line);

                // to extract time=#
                std::getline(inputFile, line);
                parseTimeValueLine(line);

                // to extract mem_heap_B=#
                std::getline(inputFile, line);
                parseMemHeapBLine(line);

                // to extract mem_heap_extra_B=#
                std::getline(inputFile, line);
                parseMemHeapExtraBLine(line);

                // to extract mem_stacks_B=#
                std::getline(inputFile, line);
                parseMemStacksBLine(line);

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

                    lines.empty();
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
                    parseHeapTreeLines(lines);
                }
            }
        }
        inputFile.close();
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
