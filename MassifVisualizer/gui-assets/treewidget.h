#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QTextBrowser>

#include <QTextBlock>

#include "snapshotlistbutton.h"
//#include "heaptreebutton.h"
#include "parse-massif-file/parser-massif.h"


class TreeWidget : public QWidget
{
    Q_OBJECT

public:
    TreeWidget(unsigned snapshotNum, ParserMassif* parser, std::string filename, QTextBrowser* textBrowser);

    QBoxLayout *buttonLayout() const;
    void setButtonLayout(QBoxLayout *buttonLayout);

private slots:
    void open_and_jump_code_file();
private:

    void createTreeLayout();

    unsigned _snapshotNum;
    ParserMassif* _parser;
    QBoxLayout* _buttonLayout;
    std::string _filename;
    QTextBrowser* _textBrowser;
    void highlightLine(unsigned lineNumber);
};

#endif // TREEWIDGET_H
