#ifndef LISTBUTTON_H
#define LISTBUTTON_H

#include<QObject>
#include<QPushButton>

#include "treewidget.h"

class ListButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ListButton(TreeWidget* treeWidget = nullptr);

    TreeWidget *treeWidget() const;

    bool isVisible() const;
    void setIsVisible(bool isVisible);

signals:

public slots:

private:
    bool _isVisible = true;
    // because of the alignment issues, it can be replaced with #pragma pack(1)
    char _padding[7];
    TreeWidget* _treeWidget;
};

#endif // LISTBUTTON_H
