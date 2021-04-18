#include "chart.h"

Chart::Chart(ParserMassif* parser) : _parser(parser)
{
    createChart();
}

Chart::Chart(std::vector<ParserMassif *> parsers, QStringList* fileNames)
    : _parsers(parsers),
      _fileNames(fileNames)
{
    createMultiChart();
}

void Chart::show_time_unit_graph()
{
    QAbstractAxis* axisXBottom = this->axes(Qt::Horizontal).back();

    if (_radioButtonTimeUnit->isChecked()) {
        this->axes(Qt::Horizontal).back()->setRange(0, _seriesTimeUnit->at(_seriesTimeUnit->count()-1).x());
        std::string timeUnit = "time unit [" + _parser->timeUnit() + "]";
        axisXBottom->setTitleText(QString::fromStdString(timeUnit.c_str()));
        this->addSeries(_seriesTimeUnit);
        _seriesTimeUnit->attachAxis(this->axes(Qt::Vertical).back());
        this->removeSeries(_seriesSnapshotNum);
        _seriesTimeUnit->attachAxis(axisXBottom);
    }
    else {
        this->axes(Qt::Horizontal).back()->setRange(0, _seriesSnapshotNum->count()-1);
        axisXBottom->setTitleText("snapshot #");
        this->addSeries(_seriesSnapshotNum);
        this->removeSeries(_seriesTimeUnit);
        _seriesSnapshotNum->attachAxis(axisXBottom);
    }
}

void Chart::createChart()
{
    _seriesSnapshotNum = new QLineSeries();
    _seriesTimeUnit = new QLineSeries();

    uint peakNum = 0;
    float peakValue = 0;

    SnapshotItem* peakSnapshot = _parser->peakSnapshot();

    if (peakSnapshot) {
        peakNum = peakSnapshot->snapshotNum();
        peakValue = peakSnapshot->memHeapB() + peakSnapshot->memHeapExtraB() + peakSnapshot->memStacksB();
    }

    for (SnapshotItem* snapshot : _parser->snapshotItems()) {
        uint xValue = snapshot->snapshotNum();
        quint64 yValue = snapshot->memHeapB() + snapshot->memHeapExtraB() + snapshot->memStacksB();
        _seriesSnapshotNum->append(xValue, yValue);
        // append(qreal), but snapshot->time() is quint64 -> out of range sometimes :(
        _seriesTimeUnit->append(snapshot->time(), yValue);
    }

    std::ostringstream title;
    std::string peakValueStr = std::to_string(peakValue/1000);

    if (_parser->exeFile().empty()) {
        title << "massif.out file does not exist!";
    }
    else {
        title << "<i>File exe [cmd]: "
              << _parser->exeFile()
              << "</i><br><b>Peak: "
              << peakValueStr.substr(0, peakValueStr.find(".")+4)
              << "KB at Snapshot #"
              << std::to_string(peakNum)
              << "</b>";
    }

    this->setTitle(QString::fromStdString(title.str()));
    this->legend()->hide();
    this->adjustSize();
    this->addSeries(_seriesSnapshotNum);
    this->createDefaultAxes();

    QValueAxis* axisX = qobject_cast<QValueAxis*>(this->axes(Qt::Horizontal)[0]);
    axisX->applyNiceNumbers();
    axisX->setLabelFormat("%d");
    axisX->setTitleText("snapshot #");

    QValueAxis* axisY = qobject_cast<QValueAxis*>(this->axes(Qt::Vertical)[0]);
    axisY->applyNiceNumbers();
    axisY->setTitleText("memory size");

    this->setTheme(QChart::ChartThemeBrownSand);

    _radioButtonTimeUnit = new QRadioButton("Time unit on x-axis");
    QObject::connect(_radioButtonTimeUnit, SIGNAL(clicked()), this, SLOT(show_time_unit_graph()));
}

void Chart::createMultiChart()
{
    std::ostringstream title;

    title << "Multiple Massif Output Files Graph";

    this->setTitle(QString::fromStdString(title.str()));
    this->legend()->setVisible(true);
    this->legend()->setAlignment(Qt::AlignBottom);
    this->adjustSize();

    int i = 0;
    for(ParserMassif *parser : _parsers){
        QLineSeries *seriesSnapshotNums = new QLineSeries();
        unsigned long index = ((_fileNames->at(i)).toStdString().find_last_of('/'));
        auto fileName = (_fileNames->at(i)).mid(static_cast<int>(index)+1);

        seriesSnapshotNums->setName(fileName);
        for (SnapshotItem* snapshot : parser->snapshotItems()) {
            uint xValue = snapshot->snapshotNum();
            quint64 yValue = snapshot->memHeapB() + snapshot->memHeapExtraB() + snapshot->memStacksB();
            seriesSnapshotNums->append(xValue, yValue);
        }
        this->addSeries(seriesSnapshotNums);
        i++;
    }
    this->createDefaultAxes();
    qobject_cast<QValueAxis*>(this->axes(Qt::Horizontal)[0])->applyNiceNumbers();
    this->axes(Qt::Vertical)[0]->setTitleText("memory size");
    this->axes(Qt::Horizontal)[0]->setTitleText("snapshot #");
    this->setTheme(QChart::ChartThemeBrownSand);
}

QRadioButton *Chart::radioButtonTimeUnit() const
{
    return _radioButtonTimeUnit;
}
