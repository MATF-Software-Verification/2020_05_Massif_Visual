#ifndef HEAPTREEBUTTON_H
#define HEAPTREEBUTTON_H

#include <QObject>
#include <QPushButton>
#include "../parse-massif-file/heaptreeitem.h"

class HeapTreeButton : public QPushButton
{
    Q_OBJECT
public:
    HeapTreeButton(std::string buttonName, HeapTreeItem* currentNode);

    HeapTreeItem *currentNode() const;

private:

    HeapTreeItem* _currentNode;

};

#endif // HEAPTREEBUTTON_H

