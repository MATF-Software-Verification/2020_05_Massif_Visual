#ifndef HEAPTREEITEM_H
#define HEAPTREEITEM_H

#include <iostream>
#include <qglobal.h>
#include <string>
#include <vector>
#include <queue>

class HeapTreeItem
{
public:
    HeapTreeItem();
    ~HeapTreeItem();

    uint numOfDirectChildren() const;
    quint64 memoryAlloc() const;
    std::string memoryAddr() const;
    std::string fileName() const;
    std::string funcName() const;
    uint lineNum() const;
    std::vector<HeapTreeItem*> children() const;
    HeapTreeItem* mother() const;

    void addChild(HeapTreeItem* node);
    void setChildren(const std::vector<HeapTreeItem*> &nodes);
    void setMother(HeapTreeItem *mother);
    void setNumOfDirectChildren(const uint &numOfDirectChildren);
    void setMemoryAlloc(const quint64 &memoryAlloc);
    void setMemoryAddr(const std::string &memoryAddr);
    void setFileName(const std::string &fileName);
    void setFuncName(const std::string &funcName);
    void setLineNum(const uint &lineNum);

    quint64 heapTreeSum();    
private:
    uint _numOfDirectChildren;
    quint64 _memoryAlloc;
    std::string _memoryAddr;
    std::string _fileName;
    std::string _funcName;
    uint _lineNum;
    std::vector<HeapTreeItem*> _children;
    HeapTreeItem* _mother;
};

#endif // HEAPTREEITEM_H
