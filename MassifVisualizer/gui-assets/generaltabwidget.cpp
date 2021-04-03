#include "generaltabwidget.h"

GeneralTabWidget::GeneralTabWidget(QWidget *parent, std::string fileName)
    : QWidget(parent),
      _fileName(fileName)
{
    _codeTextBrowser = new QTextBrowser();
    _parser = new ParserMassif(fileName);
    _parser->parseMassifOutput();
    createGraph();
}

GeneralTabWidget::GeneralTabWidget(QWidget *parent, QStringList* fileNames)
    : QWidget(parent),
      _fileNames(fileNames)
{
    _codeTextBrowser = new QTextBrowser();
    for(QString fileName : *fileNames){
        ParserMassif* parser = new ParserMassif(fileName.toStdString());
        parser->parseMassifOutput();
        _parsers.push_back(parser);
    }
    createGraph();
}

void GeneralTabWidget::change_range()
{
    int num_minLE = 0;
    int num_maxLE = 0;
    num_minLE = std::atoi(_minLE->text().toStdString().c_str());
    num_maxLE = std::atoi(_maxLE->text().toStdString().c_str());

    if(num_minLE < num_maxLE){
        _chart->axes(Qt::Horizontal).back()->setRange(num_minLE, num_maxLE);
    }
    else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Invalid axis range change error");
        msgBox.setText("MIN X Axis needs to be less than MAX X Axis.\n To be not empty, they both need.");
        msgBox.exec();
    }
}

void GeneralTabWidget::easy_visibility()
{
    ListButton *button= qobject_cast<ListButton * >(sender());

    button->treeWidget()->setVisible(button->isVisible());
    button->setIsVisible(!button->isVisible());
}


void GeneralTabWidget::createChartView()
{
    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);
}

void GeneralTabWidget::createChartBoxLayout()
{
    _chartBoxLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    if(_parser){
        _chart = new Chart(_parser);
        _chartBoxLayout->addWidget(_chart->radioButtonTimeUnit());
    }
    else if(_parsers.size() > 0){
        _chart = new Chart(_parsers);
    }

    createChartView();
    _chartBoxLayout->addWidget(_chartView);
    _chartBoxLayout->addLayout(createChangeRangeLayout());
}

QBoxLayout* GeneralTabWidget::createChangeRangeLayout()
{
    QBoxLayout *lineEditsLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    QLabel* minL = new QLabel("MIN X Axis:");
    lineEditsLayout->addWidget(minL);

    _minLE = new QLineEdit();
    _minLE->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    lineEditsLayout->addWidget(_minLE);

    QLabel* maxL = new QLabel("MAX X Axis:");
    lineEditsLayout->addWidget(maxL);

    _maxLE = new QLineEdit();
    _maxLE->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    lineEditsLayout->addWidget(_maxLE);

    QPushButton* submit = new QPushButton("submit");
    lineEditsLayout->addWidget(submit);

    QObject::connect(submit, SIGNAL(clicked()), this, SLOT(change_range()));

    return lineEditsLayout;
}

QBoxLayout *GeneralTabWidget::createTreeLayout(SnapshotListButton* generalBtn)
{
    QBoxLayout *btnLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    QString text = generalBtn->text();
    unsigned snapNum = static_cast<unsigned>(std::atoi((text.split(" ")[1]).toStdString().c_str()));
    SnapshotItem* snap = _parser->snapshotItems().at(snapNum);
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
        btnLayout->addWidget(curNodeBut);
        auto revesedChildren = tmpNode->children();
        std::reverse(revesedChildren.begin(), revesedChildren.end());

        for(auto& child : revesedChildren){
            tree.push(child);
        }
    }

    return btnLayout;

}

QBoxLayout *GeneralTabWidget::createSnapshotListLayout()
{
    QBoxLayout *generalSnapshotListLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    QBoxLayout* flowLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(flowLayout);
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaContent);
    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    scrollArea->setSizePolicy(spLeft);

    auto index = _fileName.find_last_of('/');
    auto directoryName = _fileName.substr(0, (index+1));

    std::cout << _parser->snapshotItems().size() << std::endl;
    for (SnapshotItem* snapshot : _parser->snapshotItems()) {


        //SnapshotListButton* generalPushButton = new SnapshotListButton("snapshot " + QString::number(snapshot->snapshotNum()).toStdString() , 3, "/home/student/Desktop/massif_example.c");


       ListButton* listButton;
        if(snapshot->treeType() == HeapTreeType::EMPTY){
            listButton = new ListButton();
            listButton->setDisabled(true);
            flowLayout->addWidget(listButton);
        }
        else{

            TreeWidget* treeWidget = new TreeWidget(snapshot->snapshotNum(), _parser, directoryName,_codeTextBrowser);
            treeWidget->setVisible(false);
            listButton = new ListButton(treeWidget);
            QObject::connect(listButton, SIGNAL(clicked()), this, SLOT(easy_visibility()));
            flowLayout->addWidget(listButton);
            flowLayout->addWidget(treeWidget);
            //flowLayout->addLayout(createTreeLayout(generalPushButton));
        }

        listButton->setText("snapshot " + QString::number(snapshot->snapshotNum()));

        listButton->setStyleSheet("margin: 0px 15px 0px 0px");
    }

    generalSnapshotListLayout->addWidget(scrollArea);
    return generalSnapshotListLayout;
}

QBoxLayout *GeneralTabWidget::createCodeAndTreeTabLayout()
{
    QBoxLayout *generalCreateCodeAndTreeTabLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    //_codeTextBrowser = new QTextBrowser();
    QSizePolicy spDown(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spDown.setHorizontalStretch(1);
    _codeTextBrowser->setSizePolicy(spDown);
    generalCreateCodeAndTreeTabLayout->addWidget(_codeTextBrowser);

    return generalCreateCodeAndTreeTabLayout;
}

void GeneralTabWidget::createGraph()
{
    createChartBoxLayout();

    QGraphicsView* graphicsView = new QGraphicsView();

    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(2);
    graphicsView->setSizePolicy(spLeft);
    graphicsView->setLayout(_chartBoxLayout);


    QBoxLayout *generalTabLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    generalTabLayout->addWidget(graphicsView);

    if(_parser){
        generalTabLayout->addLayout(createSnapshotListLayout());
        generalTabLayout->addLayout(createCodeAndTreeTabLayout());
    }

    this->setLayout(generalTabLayout);
}
