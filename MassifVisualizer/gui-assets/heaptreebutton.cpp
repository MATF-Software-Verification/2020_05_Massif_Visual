#include "heaptreebutton.h"

HeapTreeButton::HeapTreeButton(std::string buttonName, HeapTreeItem *currentNode)
{
    this->setText(QString::fromStdString(buttonName));
    _currentNode = currentNode;
}

HeapTreeItem *HeapTreeButton::currentNode() const
{
    return _currentNode;
}
