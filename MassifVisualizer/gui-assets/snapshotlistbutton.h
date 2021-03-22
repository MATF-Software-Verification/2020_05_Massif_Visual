#ifndef SNAPSHOTLISTBUTTON_H
#define SNAPSHOTLISTBUTTON_H

#include <QObject>
#include <QPushButton>

class SnapshotListButton : public QPushButton
{
    Q_OBJECT
public:
    SnapshotListButton(std::string buttonName, unsigned _lineNumber, std::string _codeFilename);

    std::string getCodeFileName();
    unsigned getLineNumber();

private:

    unsigned _lineNumber;
    std::string _codeFilename;
};

#endif // SNAPSHOTLISTBUTTON_H
