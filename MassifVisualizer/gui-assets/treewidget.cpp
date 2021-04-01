#include "treewidget.h"

TreeWidget::TreeWidget(unsigned snapshotNum, ParserMassif *parser, std::string filename, QTextBrowser* textBrowser)
{
    _snapshotNum = snapshotNum;
    _parser = parser;
    _filename = filename;
    _textBrowser = textBrowser;
    createTreeLayout();
    this->setLayout(_buttonLayout);
}

void TreeWidget::open_and_jump_code_file()
{
    SnapshotListButton *button= qobject_cast<SnapshotListButton * >(sender());


    auto directoryName = _filename;
    std::string fileName =directoryName + button->getCodeFileName();
    std::cout << fileName << std::endl;
    unsigned jumpLine = button->getLineNumber()-1;

    if (fileName.empty())
        return;

    std::ifstream in(fileName);
    /*QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       return;
    }

    QString text;
    QString line;
    int i = 1;
    QString numberedLine;

    while (!file.atEnd()) {
        line = file.readLine();
        numberedLine = "<font color=\"DeepPink\">" + QString::number(i) + " </font>";
        line = "<pre>" + line + "</pre>";
        _textBrowser->insertHtml(numberedLine);
        _textBrowser->insertHtml(line);
        _textBrowser->insertPlainText("\n");
        i++;

    }

    file.close();
    highlightLine(jumpLine);*/

    std::string text;
    std::string line;
    int i = 1;
    std::string numberedLine;

    while (std::getline(in, line)) {
        numberedLine = QString::number(i).toStdString() + " ";
        line.append("\n");
        text.append(numberedLine);
        text.append(line);
        i++;
    }

    in.close();
    QString code = QString::fromStdString(text);
    _textBrowser->setText(code);
    highlightLine(jumpLine);

}

void TreeWidget::highlightLine(unsigned lineNumber)
{
    QTextCursor coursor(_textBrowser->document()->findBlockByLineNumber(static_cast<int>(lineNumber)));
    QTextBlockFormat frmt = coursor.blockFormat();
    frmt.setBackground(QBrush(QColor(255, 128, 0)));
    coursor.setBlockFormat(frmt);
    _textBrowser->setTextCursor(coursor);
}

void TreeWidget::createTreeLayout()
{
    _buttonLayout = new QBoxLayout(QBoxLayout::TopToBottom);


    SnapshotItem* snap = _parser->snapshotItems().at(_snapshotNum);
    QString numOfChildren = "n"+ QString::number(snap->heapTreeItem()->numOfDirectChildren());
    HeapTreeItem* root = snap->heapTreeItem();
    std::stack<HeapTreeItem*> tree;
    tree.push(root);
    bool disabledBtn = true;

    while(!tree.empty()){
        HeapTreeItem* tmpNode = tree.top();
        tree.pop();
        QString BUTname = "n" + QString::number(tmpNode->numOfDirectChildren()) + ": ";
        BUTname.append(QString::number(tmpNode->memoryAlloc()) + " " + QString::fromStdString(tmpNode->funcName()));
        SnapshotListButton* curNodeBut = new SnapshotListButton(BUTname.toStdString(), tmpNode->lineNum(), tmpNode->fileName());
        if(disabledBtn){
            curNodeBut->setDisabled(true);
            curNodeBut->setStyleSheet("margin : 0px 0px 0px " + QString::number(tmpNode->indentation()*10) + "px; font-size: 11px");
            disabledBtn = false;
        }
        else {
            QObject::connect(curNodeBut, SIGNAL(clicked()), this, SLOT(open_and_jump_code_file()));
            curNodeBut->setStyleSheet("background-color:#ffad33; margin : 0px 0px 0px " + QString::number(tmpNode->indentation()*10) + "px; font-size: 11px");
        }

        _buttonLayout->addWidget(curNodeBut);
        auto revesedChildren = tmpNode->children();
        std::reverse(revesedChildren.begin(), revesedChildren.end());

        for(auto& child : revesedChildren){
            tree.push(child);
        }
    }
}

QBoxLayout *TreeWidget::buttonLayout() const
{
    return _buttonLayout;
}

void TreeWidget::setButtonLayout(QBoxLayout *buttonLayout)
{
    _buttonLayout = buttonLayout;
}
