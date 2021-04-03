#include "chart.h"

Chart::Chart(ParserMassif* parser) : _parser(parser)
{
    createChart();
}

Chart::Chart(std::vector<ParserMassif *> parsers) : _parsers(parsers)
{
    createMultiChart();
}

void Chart::show_time_unit_graph()
{
    auto axisXBottom = this->axes(Qt::Horizontal).back();

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

    std::ostringstream title;
    std::string peakValueStr = std::to_string(peakValue/1000);

    title << "<i>File exe: ./"
          << _parser->exeFile()
          << "</i><br><b>Peak: "
          << peakValueStr.substr(0, peakValueStr.find(".")+4)
          << "KB at Snapshot #"
          << std::to_string(peakNum)
          << "</b>";

    this->setTitle(QString::fromStdString(title.str()));
    this->legend()->hide();
    this->adjustSize();

    QValueAxis* axisYLeft = new QValueAxis();
    axisYLeft->applyNiceNumbers();
    axisYLeft->setMin(0);
    axisYLeft->setLabelFormat("%d");
    axisYLeft->setTitleText("memory size");

    QValueAxis* axisXBottom = new QValueAxis();
    axisXBottom->applyNiceNumbers();
    axisXBottom->setMin(0);
    axisXBottom->setLabelFormat("%d");

    this->addAxis(axisYLeft, Qt::AlignLeft);
    this->addAxis(axisXBottom, Qt::AlignBottom);

    this->addSeries(_seriesSnapshotNum);

    axisXBottom->setTitleText("snapshot #");
    _seriesSnapshotNum->attachAxis(axisYLeft);
    _seriesSnapshotNum->attachAxis(axisXBottom);

    this->setTheme(QChart::ChartThemeBrownSand);

    _radioButtonTimeUnit = new QRadioButton("Time unit on x-axis");
    QObject::connect(_radioButtonTimeUnit, SIGNAL(clicked()), this, SLOT(show_time_unit_graph()));

}

void Chart::createMultiChart()
{
    std::ostringstream title;

    title << "TODO";

    this->setTitle(QString::fromStdString(title.str()));
    this->legend()->hide();
    this->adjustSize();

    QValueAxis* axisYLeft = new QValueAxis();
    axisYLeft->applyNiceNumbers();
    axisYLeft->setMin(0);
    axisYLeft->setLabelFormat("%d");
    axisYLeft->setTitleText("memory size");

    QValueAxis* axisXBottom = new QValueAxis();
    axisXBottom->applyNiceNumbers();
    axisXBottom->setMin(0);
    axisXBottom->setLabelFormat("%d");

    this->addAxis(axisYLeft, Qt::AlignLeft);
    this->addAxis(axisXBottom, Qt::AlignBottom);

    for(ParserMassif *parser : _parsers){
        QLineSeries *seriesSnapshotNums = new QLineSeries();

        for (SnapshotItem* snapshot : parser->snapshotItems()) {
            uint xValue = snapshot->snapshotNum();
            quint64 yValue = snapshot->memHeapB() + snapshot->memHeapExtraB() + snapshot->memStacksB();
            seriesSnapshotNums->append(xValue, yValue);
        }
        this->addSeries(seriesSnapshotNums);
    }
    this->createDefaultAxes();
    axisXBottom->setTitleText("snapshot #");
    this->setTheme(QChart::ChartThemeBrownSand);
}

QRadioButton *Chart::radioButtonTimeUnit() const
{
    return _radioButtonTimeUnit;
}
