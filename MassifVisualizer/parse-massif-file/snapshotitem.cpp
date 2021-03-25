#include "snapshotitem.h"

SnapshotItem::SnapshotItem()
    : _snapshotNum(0),
      _time(0),
      _memHeapB(0),
      _memHeapExtraB(0),
      _memStacksB(0),
      _treeType(HeapTreeType::EMPTY),
      _heapTreeItem(nullptr)
{

}

SnapshotItem::~SnapshotItem()
{
    delete _heapTreeItem;
}

uint SnapshotItem::snapshotNum() const
{
    return _snapshotNum;
}

void SnapshotItem::setSnapshotNum(const uint &snapshotNum)
{
    _snapshotNum = snapshotNum;
}

double SnapshotItem::time() const
{
    return _time;
}

void SnapshotItem::setTime(double time)
{
    _time = time;
}

quint64 SnapshotItem::memHeapB() const
{
    return _memHeapB;
}

void SnapshotItem::setMemHeapB(const quint64 &memHeapB)
{
    _memHeapB = memHeapB;
}

quint64 SnapshotItem::memHeapExtraB() const
{
    return _memHeapExtraB;
}

void SnapshotItem::setMemHeapExtraB(const quint64 &memHeapExtraB)
{
    _memHeapExtraB = memHeapExtraB;
}

quint64 SnapshotItem::memStacksB() const
{
    return _memStacksB;
}

void SnapshotItem::setMemStacksB(const quint64 &memStacksB)
{
    _memStacksB = memStacksB;
}

HeapTreeItem *SnapshotItem::heapTreeItem() const
{
    return _heapTreeItem;
}

void SnapshotItem::setHeapTreeItem(HeapTreeItem *heapTreeItem)
{
    _heapTreeItem = heapTreeItem;
}

void SnapshotItem::setTreeType(const HeapTreeType &treeType)
{
    _treeType = treeType;
}

