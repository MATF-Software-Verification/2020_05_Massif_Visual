#include "generaltabwidget.h"

GeneralTabWidget::GeneralTabWidget(QWidget *parent) : QWidget(parent)
{
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

void GeneralTabWidget::highlightLine(unsigned lineNumber)
{
    QTextCursor coursor(_codeTextBrowser->document()->findBlockByLineNumber(static_cast<int>(lineNumber)));
    QTextBlockFormat frmt = coursor.blockFormat();
    frmt.setBackground(QBrush(Qt::yellow));
    coursor.setBlockFormat(frmt);
    _codeTextBrowser->setTextCursor(coursor);
}

void GeneralTabWidget::createChart()
{
    //TODO: this will look completely different
    QLineSeries *series = new QLineSeries();
    series->append(0, 10);
    series->append(1, 11);
    series->append(2, 12);
    series->append(3, 100);
    series->append(4, 101);
    series->append(5, 100);
    series->append(6, 34);

    _chart = new QChart();
    _chart->legend()->hide();
    _chart->addSeries(series);
    _chart->createDefaultAxes();
}

void GeneralTabWidget::createChartView()
{
    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);
}

void GeneralTabWidget::createChartBoxLayout()
{
    _chartBoxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
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
    SnapshotListButton* generalPushButton1 = new SnapshotListButton("button 1", 3, "/home/student/Desktop/massif_example.c");


    QObject::connect(generalPushButton1, SIGNAL(clicked()), this, SLOT(open_and_jump_code_file()));

    generalSnapshotListLayout->addWidget(generalPushButton1);

    return generalSnapshotListLayout;
}

void GeneralTabWidget::createGraph()
{
    createChart();
    createChartView();
    createChartBoxLayout();

    QGraphicsView* graphicsView = new QGraphicsView();
    graphicsView->setLayout(_chartBoxLayout);
    QBoxLayout *generalTabLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    generalTabLayout->addWidget(graphicsView);
    generalTabLayout->addLayout(createSnapshotListLayout());
    _codeTextBrowser = new QTextBrowser();
    generalTabLayout->addWidget(_codeTextBrowser);
    this->setLayout(generalTabLayout);
}

