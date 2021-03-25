#include "heaptreeitem.h"

HeapTreeItem::HeapTreeItem()
    : _numOfDirectChildren(0),
      _memoryAlloc(0),
      _fileName(""),
      _funcName(""),
      _lineNum(0),
      _mother(nullptr)
{

}

HeapTreeItem::~HeapTreeItem()
{
    for (HeapTreeItem* child: _children) {
        delete child;
        child = nullptr;
    }
    _children.clear();
    // we are not going to call delete _mother
    // because it has already been called
    _mother = nullptr;
}

uint HeapTreeItem::numOfDirectChildren() const
{
    return _numOfDirectChildren;
}

void HeapTreeItem::setNumOfDirectChildren(const uint &numOfDirectChildren)
{
    _numOfDirectChildren = numOfDirectChildren;
}

quint64 HeapTreeItem::memoryAlloc() const
{
    return _memoryAlloc;
}

void HeapTreeItem::setMemoryAlloc(const quint64 &memoryAlloc)
{
    _memoryAlloc = memoryAlloc;
}

std::string HeapTreeItem::memoryAddr() const
{
    return _memoryAddr;
}

void HeapTreeItem::setMemoryAddr(const std::string &memoryAddr)
{
    _memoryAddr = memoryAddr;
}

std::string HeapTreeItem::fileName() const
{
    return _fileName;
}

void HeapTreeItem::setFileName(const std::string &fileName)
{
    _fileName = fileName;
}

std::string HeapTreeItem::funcName() const
{
    return _funcName;
}

void HeapTreeItem::setFuncName(const std::string &funcName)
{
    _funcName = funcName;
}

uint HeapTreeItem::lineNum() const
{
    return _lineNum;
}

void HeapTreeItem::setLineNum(const uint &lineNum)
{
    _lineNum = lineNum;
}

std::vector<HeapTreeItem *> HeapTreeItem::children() const
{
    return _children;
}

void HeapTreeItem::setChildren(const std::vector<HeapTreeItem *> &nodes)
{
    _children = nodes;
}

HeapTreeItem *HeapTreeItem::mother() const
{
    return _mother;
}

void HeapTreeItem::setMother(HeapTreeItem *mother)
{
    _mother = mother;
}

void HeapTreeItem::addChild(HeapTreeItem *node)
{
    node->_mother = this;
    _children.push_back(node);
}

// OVO NAM NE TREBA NI ZA STA
quint64 HeapTreeItem::heapTreeSum()
{
    quint64 sum = 0;

    std::queue<HeapTreeItem*> queue;
    queue.push(this);

    while (!queue.empty()) {
        auto n = queue.size();
        while (n > 0) {
            HeapTreeItem* tmpNode = queue.front();
            queue.pop();
            sum += tmpNode->memoryAlloc();

            for (auto child : tmpNode->children()) {
                queue.push(child);
            }
            n--;
        }
    }
    return sum;
}
