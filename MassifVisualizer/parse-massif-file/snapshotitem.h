#ifndef SNAPSHOTITEM_H
#define SNAPSHOTITEM_H

#include <qglobal.h>
#include "./heaptreeitem.h"

enum class HeapTreeType {
    EMPTY,
    DETAILED,
    PEAK
};

class SnapshotItem
{
public:
    SnapshotItem();
    ~SnapshotItem();

    uint snapshotNum() const;
    void setSnapshotNum(const uint &snapshotNum);

    quint64 time() const;
    void setTime(quint64 time);

    quint64 memHeapB() const;
    void setMemHeapB(const quint64 &memHeapB);

    quint64 memHeapExtraB() const;
    void setMemHeapExtraB(const quint64 &memHeapExtraB);

    quint64 memStacksB() const;
    void setMemStacksB(const quint64 &memStacksB);

    HeapTreeItem *heapTreeItem() const;
    void setHeapTreeItem(HeapTreeItem *heapTreeItem);

    HeapTreeType treeType() const;
    void setTreeType(const HeapTreeType &treeType);

private:
    uint _snapshotNum;
    quint64 _time;
    //quint64 = unsigned long long int
    quint64 _memHeapB;
    quint64 _memHeapExtraB;
    quint64 _memStacksB;

    HeapTreeType _treeType;
    HeapTreeItem* _heapTreeItem;
};

#endif // SNAPSHOTITEM_H
