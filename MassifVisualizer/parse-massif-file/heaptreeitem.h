#ifndef HEAPTREEITEM_H
#define HEAPTREEITEM_H

#include <qglobal.h>
#include <string>
#include <vector>

class HeapTreeItem
{
public:
    HeapTreeItem();
    ~HeapTreeItem();

private:
    uint _numOfDirectChildren;
    quint64 _memoryAlloc;
    std::string _fileName;
    std::string _funcName;
    uint _lineNum;
    std::vector<HeapTreeItem*> _children;
    HeapTreeItem* _mother;
};

#endif // HEAPTREEITEM_H
