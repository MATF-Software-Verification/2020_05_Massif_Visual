#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QTextBrowser>

#include <QTextBlock>

#include "snapshotlistbutton.h"
#include "parse-massif-file/parser-massif.h"
#include "../helper-functions.h"

class TreeWidget : public QWidget
{
    Q_OBJECT

public:
    TreeWidget(unsigned snapshotNum, ParserMassif* parser, std::string filename, QTextBrowser* textBrowser);

    QBoxLayout *buttonLayout() const;
    void setButtonLayout(QBoxLayout *buttonLayout);
    void updateBtnTheme();

private slots:
    void open_and_jump_code_file();
private:

    void createTreeLayout();
    void highlightLine(unsigned lineNum);


    unsigned _snapshotNum;
    ParserMassif* _parser;
    QBoxLayout* _buttonLayout;
    std::string _filename;
    QTextBrowser* _textBrowser;
    QColor _penColor = QColor(255, 128, 0);
};

#endif // TREEWIDGET_H
