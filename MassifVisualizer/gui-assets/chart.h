#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QRadioButton>

#include "parse-massif-file/parser-massif.h"

QT_CHARTS_USE_NAMESPACE

class Chart : public QChart
{
   Q_OBJECT
public:
    Chart(ParserMassif* parser);
    Chart(std::vector<ParserMassif*> parsers, QStringList* fileNames);

    QRadioButton *radioButtonTimeUnit() const;

private slots:
    void show_time_unit_graph();
private:
    void createChart();
    void createMultiChart();

    QLineSeries* _seriesSnapshotNum;
    QLineSeries* _seriesTimeUnit;
    ParserMassif* _parser;
    std::vector<ParserMassif*> _parsers;
    QStringList* _fileNames;

    QRadioButton* _radioButtonTimeUnit;
};

#endif // CHART_H
