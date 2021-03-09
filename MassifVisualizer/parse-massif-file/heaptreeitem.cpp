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
    //TODO: delete all children and test that
}

//TODO: define functions for inserting child and setting parent
