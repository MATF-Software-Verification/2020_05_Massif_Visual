#include "generaltabwidget.h"

GeneralTabWidget::GeneralTabWidget(QWidget *parent, std::string fileName)
    : QWidget(parent),
      _fileName(fileName)
{
    _parser = new ParserMassif(fileName);
    _parser->parseMassifOutput();
    createGraph();
}

void GeneralTabWidget::changeRange()
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

void GeneralTabWidget::open_and_jump_code_file()
{
    SnapshotListButton *button= qobject_cast<SnapshotListButton * >(sender());

    std::string fileName = button->getCodeFileName();
    unsigned jumpLine = button->getLineNumber();

    if (fileName.empty())
        return;

    std::ifstream in(fileName);

    std::string text;
    std::string line;
    while (std::getline(in, line)) {

        line.append("\n");
        text.append(line);
    }

    in.close();
    QString code = QString::fromStdString(text);
    _codeTextBrowser->setText(code);
    highlightLine(jumpLine);
}

void GeneralTabWidget::showTimeUnitGraph()
{
    auto axisXBottom = _chart->axes(Qt::Horizontal).back();

    if (_radioButtonTimeUnit->isChecked()) {
        _chart->axes(Qt::Horizontal).back()->setRange(0, _seriesTimeUnit->at(_seriesTimeUnit->count()-1).x());
        std::string timeUnit = "time unit [" + _parser->timeUnit() + "]";
        axisXBottom->setTitleText(QString::fromStdString(timeUnit.c_str()));
        _chart->addSeries(_seriesTimeUnit);
        _seriesTimeUnit->attachAxis(_chart->axes(Qt::Vertical).back());
        _chart->removeSeries(_seriesSnapshotNum);
        _seriesTimeUnit->attachAxis(axisXBottom);
    }
    else {
        _chart->axes(Qt::Horizontal).back()->setRange(0, _seriesSnapshotNum->count()-1);
        axisXBottom->setTitleText("snapshot #");
        _chart->addSeries(_seriesSnapshotNum);
        _chart->removeSeries(_seriesTimeUnit);
        _seriesSnapshotNum->attachAxis(axisXBottom);
    }
}

void GeneralTabWidget::create_my_children()
{
    HeapTreeButton *button= qobject_cast<HeapTreeButton * >(sender());
    //QBoxLayout* btnLayout = qobject_cast<QBoxLayout * >(button->layout());
    HeapTreeItem* currentNode = button->currentNode();

    for(HeapTreeItem* child: currentNode->children()) {
        QString buttonName = "n" + QString::number(child->numOfDirectChildren());
        HeapTreeButton* node = new HeapTreeButton(buttonName.toStdString(), child);
        _treeBoxLayout->addWidget(node);
        QObject::connect(node, SIGNAL(clicked()), this, SLOT(create_my_children()));
        _treeBoxLayout->setAlignment(node, Qt::AlignTop);
    }
}

void GeneralTabWidget::create_tree_root()
{
    QPushButton *button= qobject_cast<QPushButton * >(sender());
    QString text = button->text();
    unsigned snapNum = static_cast<unsigned>(std::atoi((text.split(" ")[1]).toStdString().c_str()));
    SnapshotItem* snap = _parser->snapshotItems().at(snapNum);
    QString numOfChildren = "n"+ QString::number(snap->heapTreeItem()->numOfDirectChildren());

    HeapTreeButton* tree_root = new HeapTreeButton(numOfChildren.toStdString(), snap->heapTreeItem());
    _treeBoxLayout->addWidget(tree_root);
    QObject::connect(tree_root, SIGNAL(clicked()), this, SLOT(create_my_children()));
    _treeBoxLayout->setAlignment(tree_root, Qt::AlignTop);
}

void GeneralTabWidget::highlightLine(unsigned lineNumber)
{
    QTextCursor coursor(_codeTextBrowser->document()->findBlockByLineNumber(static_cast<int>(lineNumber)));
    QTextBlockFormat frmt = coursor.blockFormat();
    frmt.setBackground(QBrush(QColor(255, 128, 0)));
    coursor.setBlockFormat(frmt);
    _codeTextBrowser->setTextCursor(coursor);
}

void GeneralTabWidget::createChart()
{
    _seriesSnapshotNum = new QLineSeries();
    _seriesTimeUnit = new QLineSeries();

    _parser->parseMassifOutput();
    uint peakNum = 0;
    float peakValue = 0;

    for (SnapshotItem* snapshot : _parser->snapshotItems()) {
        uint xValue = snapshot->snapshotNum();
        quint64 yValue = snapshot->memHeapB() + snapshot->memHeapExtraB() + snapshot->memStacksB();
        _seriesSnapshotNum->append(xValue, yValue);
        _seriesTimeUnit->append(snapshot->time(), yValue);

        if (snapshot->treeType() == HeapTreeType::PEAK) {
            peakNum = xValue;
            peakValue = yValue;
        }
    }

    _chart = new QChart();
    std::ostringstream title;
    std::string peakValueStr = std::to_string(peakValue/1000);

    title << "<i>File exe: ./"
          << _parser->exeFile()
          << "</i><br><b>Peak: "
          << peakValueStr.substr(0, peakValueStr.find(".")+4)
          << "KB at Snapshot #"
          << std::to_string(peakNum)
          << "</b>";

    _chart->setTitle(QString::fromStdString(title.str()));
    _chart->legend()->hide();
    _chart->adjustSize();

    QValueAxis* axisYLeft = new QValueAxis();
    axisYLeft->applyNiceNumbers();
    axisYLeft->setMin(0);
    axisYLeft->setLabelFormat("%d");
    axisYLeft->setTitleText("memory size");

    QValueAxis* axisXBottom = new QValueAxis();
    axisXBottom->applyNiceNumbers();
    axisXBottom->setMin(0);
    axisXBottom->setLabelFormat("%d");

    _chart->addAxis(axisYLeft, Qt::AlignLeft);
    _chart->addAxis(axisXBottom, Qt::AlignBottom);

    _chart->addSeries(_seriesSnapshotNum);

    axisXBottom->setTitleText("snapshot #");
    _seriesSnapshotNum->attachAxis(axisYLeft);
    _seriesSnapshotNum->attachAxis(axisXBottom);

    //_chart->createDefaultAxes();
    _chart->setTheme(QChart::ChartThemeBrownSand);
}

void GeneralTabWidget::createChartView()
{
    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);
}

void GeneralTabWidget::createChartBoxLayout()
{
    _chartBoxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    _radioButtonTimeUnit = new QRadioButton("Time unit on x-axis");
    QObject::connect(_radioButtonTimeUnit, SIGNAL(clicked()), this, SLOT(showTimeUnitGraph()));

    _chartBoxLayout->addWidget(_radioButtonTimeUnit);

    createChart();
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

    QObject::connect(submit, SIGNAL(clicked()), this, SLOT(changeRange()));

    return lineEditsLayout;
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


    std::cout << _parser->snapshotItems().size() << std::endl;
    for (SnapshotItem* snapshot : _parser->snapshotItems()) {

        SnapshotListButton* generalPushButton = new SnapshotListButton("snapshot " + QString::number(snapshot->snapshotNum()).toStdString() , 3, "/home/student/Desktop/massif_example.c");

        generalPushButton->setStyleSheet("margin: 0px 15px 0px 0px");
        if(snapshot->treeType() == HeapTreeType::EMPTY){
            generalPushButton->setDisabled(true);
        }
        else{
            QObject::connect(generalPushButton, SIGNAL(clicked()), this, SLOT(create_tree_root()));
        }
        flowLayout->addWidget( generalPushButton );

    }

    generalSnapshotListLayout->addWidget(scrollArea);
    return generalSnapshotListLayout;
}

QBoxLayout *GeneralTabWidget::createTreeButtonsLayout()
{
    QBoxLayout *generalCreateTreeButtonsLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    QPushButton* tree_root = new QPushButton("tree_root");
    generalCreateTreeButtonsLayout->addWidget(tree_root);
    QObject::connect(tree_root, SIGNAL(clicked()), this, SLOT(create_my_children()));
    generalCreateTreeButtonsLayout->setAlignment(tree_root, Qt::AlignTop);
    return generalCreateTreeButtonsLayout;
}

QBoxLayout *GeneralTabWidget::createCodeAndTreeTabLayout()
{
    QBoxLayout *generalCreateCodeAndTreeTabLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    QWidget *tree = new QWidget();
    QSizePolicy spUp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spUp.setVerticalStretch(2);
    tree->setSizePolicy(spUp);
    _treeBoxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    tree->setLayout(_treeBoxLayout);
    generalCreateCodeAndTreeTabLayout->addWidget(tree);

    _codeTextBrowser = new QTextBrowser();
    QSizePolicy spDown(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spDown.setVerticalStretch(1);
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
    generalTabLayout->addLayout(createSnapshotListLayout());

    generalTabLayout->addLayout(createCodeAndTreeTabLayout());
    this->setLayout(generalTabLayout);
}

