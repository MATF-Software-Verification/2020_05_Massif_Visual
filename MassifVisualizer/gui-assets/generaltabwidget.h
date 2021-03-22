#ifndef GENERALTABWIDGET_H
#define GENERALTABWIDGET_H

#include <fstream>

#include <QWidget>
#include <QtCharts/QChartView>
#include <QLineSeries>
#include <QBoxLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextBrowser>
#include <QTextCursor>
#include <QTextBlock>

#include "snapshotlistbutton.h"

QT_CHARTS_USE_NAMESPACE

class GeneralTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralTabWidget(QWidget *parent = nullptr);

signals:

public slots:

private slots:

    void changeRange();
    void open_and_jump_code_file();

private:

    void createChart();
    void createChartView();
    void createChartBoxLayout();

    QBoxLayout *createChangeRangeLayout();
    QBoxLayout *createSnapshotListLayout();

    void createGraph();

    QChart* _chart;
    QChartView* _chartView;
    QBoxLayout* _chartBoxLayout;
    QLineEdit* _minLE;
    QLineEdit* _maxLE;
    QTextBrowser* _codeTextBrowser;

    std::string _fileName;

    void highlightLine(unsigned lineNumber);
};

#endif // GENERALTABWIDGET_H

