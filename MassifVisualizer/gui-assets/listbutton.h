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

    TreeWidget* _treeWidget;
    bool _isVisible = true;
};

#endif // LISTBUTTON_H
