#include "listbutton.h"

ListButton::ListButton(TreeWidget* treeWidget) : QPushButton()
{

    _treeWidget = treeWidget;
}

TreeWidget *ListButton::treeWidget() const
{
    return _treeWidget;
}

bool ListButton::isVisible() const
{
    return _isVisible;
}

void ListButton::setIsVisible(bool isVisible)
{
    _isVisible = isVisible;
}
