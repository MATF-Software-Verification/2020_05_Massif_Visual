#include "generaltabwidget.h"

GeneralTabWidget::GeneralTabWidget(QWidget *parent, std::string fileName, ParserMassif* parser)
    : QWidget(parent),
      _fileName(fileName),
      _parser(parser)
{
    _codeTextBrowser = new QTextBrowser();
    createGraph();
}

GeneralTabWidget::GeneralTabWidget(QWidget *parent, QStringList* fileNames, std::vector<ParserMassif*> parsers)
    : QWidget(parent),
      _fileNames(fileNames),
      _parsers(parsers)
{
    _codeTextBrowser = new QTextBrowser();
    createGraph();
}

GeneralTabWidget::~GeneralTabWidget()
{
    delete _generalTabLayout;
    delete _chartBoxLayout;

    delete _chart;
    delete _codeTextBrowser;
    delete _parser;

    for (ParserMassif* parser: _parsers) {
        delete parser;
    }
    _parsers.clear();

    for (TreeWidget* treeWidget: _treeWidgets) {
        delete treeWidget;
    }
    _treeWidgets.clear();
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
        QString title = "Invalid axis range change error " + QString::fromUtf8("\xF0\x9F\x90\xA2");
        QString text = "MIN X Axis needs to be less than MAX X Axis.\n To be not empty, they both need.";
        QMessageBox msgBox;
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        setTheme(qobject_cast<QWidget*>(&msgBox));
        msgBox.exec();
    }
}

void GeneralTabWidget::easy_visibility()
{
    ListButton *button = qobject_cast<ListButton *>(sender());

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

    if(_parser) {
        _chart = new Chart(_parser);
        _chartBoxLayout->addWidget(_chart->radioButtonTimeUnit());
    }
    else if(_parsers.size() > 0){
        _chart = new Chart(_parsers, _fileNames);
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

void GeneralTabWidget::initListLayout(QScrollArea *scrollArea)
{
    _flowLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    _scrollAreaContent = new QWidget;
    _scrollAreaContent->setLayout(_flowLayout);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(_scrollAreaContent);

    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    scrollArea->setSizePolicy(spLeft);
}

QBoxLayout *GeneralTabWidget::createPeakListLayout()
{
    QBoxLayout *generalPeakListLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    QScrollArea* scrollArea = new QScrollArea;
    initListLayout(scrollArea);

    ListButton* listButton;
    int i = 0;
    _treeWidgets.clear();
    for(ParserMassif* parser : _parsers){
        int index = static_cast<int>((_fileNames->at(i)).toStdString().find_last_of('/'));
        auto directoryName = (_fileNames->at(i)).toStdString().substr(0, (static_cast<unsigned long>(index)+1));
        SnapshotItem* currPeak = parser->peakSnapshot();

        if (currPeak) {
            TreeWidget* treeWidget = new TreeWidget(currPeak->snapshotNum(), parser, directoryName, _codeTextBrowser);
            treeWidget->setVisible(false);
            listButton = new ListButton(treeWidget);
            QObject::connect(listButton, SIGNAL(clicked()), this, SLOT(easy_visibility()));
            _flowLayout->addWidget(listButton, 0, Qt::AlignTop);
            _flowLayout->addWidget(treeWidget, 1, Qt::AlignTop);
            QString listBtnName = "peak " + QString::number(currPeak->snapshotNum()) + " ";
            listButton->setText(listBtnName.append((_fileNames->at(i)).mid(index+1)));
            listButton->setStyleSheet("margin: 0px 15px 0px 0px");
            _treeWidgets.push_back(treeWidget);
        }
        i++;
    }

    generalPeakListLayout->addWidget(scrollArea);
    return generalPeakListLayout;
}

QBoxLayout *GeneralTabWidget::createSnapshotListLayout()
{
    QBoxLayout *generalSnapshotListLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    QScrollArea* scrollArea = new QScrollArea;
    initListLayout(scrollArea);

    auto index = _fileName.find_last_of('/');
    auto directoryName = _fileName.substr(0, (index+1));

    _treeWidgets.clear();
    for (SnapshotItem* snapshot : _parser->snapshotItems()) {

       ListButton* listButton;

        if(snapshot->treeType() == HeapTreeType::EMPTY){
            listButton = new ListButton();
            listButton->setDisabled(true);
            _flowLayout->addWidget(listButton, 0, Qt::AlignTop);
        }
        else{
            TreeWidget* treeWidget = new TreeWidget(snapshot->snapshotNum(), _parser, directoryName, _codeTextBrowser);
            treeWidget->setVisible(false);
            listButton = new ListButton(treeWidget);
            QObject::connect(listButton, SIGNAL(clicked()), this, SLOT(easy_visibility()));
            _flowLayout->addWidget(listButton, 0, Qt::AlignTop);
            _flowLayout->addWidget(treeWidget, 0, Qt::AlignTop);
            _treeWidgets.push_back(treeWidget);
        }
        listButton->setText("snapshot " + QString::number(snapshot->snapshotNum()));
        listButton->setStyleSheet("margin: 0px 15px 0px 0px");
    }
    generalSnapshotListLayout->addWidget(scrollArea);
    return generalSnapshotListLayout;
}

QBoxLayout *GeneralTabWidget::createCodeLayout()
{
    QBoxLayout *generalCreateCodeAndTreeTabLayout = new QBoxLayout(QBoxLayout::TopToBottom);
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

    _generalTabLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    _generalTabLayout->addWidget(graphicsView);

    if(_parser){
        _generalTabLayout->addLayout(createSnapshotListLayout());
    }
    else if(_parsers.size() > 0){
        _generalTabLayout->addLayout(createPeakListLayout());
    }

    _generalTabLayout->addLayout(createCodeLayout());
    this->setLayout(_generalTabLayout);
}

std::vector<TreeWidget *> GeneralTabWidget::treeWidgets() const
{
    return _treeWidgets;
}
