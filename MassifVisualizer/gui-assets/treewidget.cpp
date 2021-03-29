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

    std::cout<<"a"<<std::endl;

    if (fileName.empty())
        return;

    std::ifstream in(fileName);

    std::string text;
    std::string line;    while (std::getline(in, line)) {

        line.append("\n");
        text.append(line);
    }

    std::cout<<"b"<<std::endl;

    in.close();
    QString code = QString::fromStdString(text);
    std::cout<<"ba"<<std::endl;
    _textBrowser->setText(code);
    std::cout<<"bb"<<std::endl;
    highlightLine(jumpLine);

    std::cout<<"c"<<std::endl;
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


    while(!tree.empty()){
        HeapTreeItem* tmpNode = tree.top();
        tree.pop();
        QString BUTname = "n" + QString::number(tmpNode->numOfDirectChildren());
        std::cout << tmpNode->lineNum() << " " << tmpNode->fileName() << std::endl;
        SnapshotListButton* curNodeBut = new SnapshotListButton(BUTname.toStdString(), tmpNode->lineNum(), tmpNode->fileName());
        QObject::connect(curNodeBut, SIGNAL(clicked()), this, SLOT(open_and_jump_code_file()));
        curNodeBut->setStyleSheet("margin : 0px 0px 0px " + QString::number(tmpNode->indentation()*10) + "px");
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
