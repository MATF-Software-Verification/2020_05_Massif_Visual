#include "snapshotlistbutton.h"

SnapshotListButton::SnapshotListButton(std::string buttonName, unsigned lineNumber, std::string codeFilename)
{
    this->setText(QString::fromStdString(buttonName));
    _codeFilename = codeFilename;
    _lineNumber = lineNumber;
}

std::string SnapshotListButton::getCodeFileName()
{
    return _codeFilename;
}

unsigned SnapshotListButton::getLineNumber()
{
    return _lineNumber;
}
