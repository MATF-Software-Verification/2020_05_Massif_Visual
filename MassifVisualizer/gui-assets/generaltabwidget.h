#ifndef GENERALTABWIDGET_H
#define GENERALTABWIDGET_H

#include <fstream>

#include <QWidget>
#include <QChartView>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QLineSeries>
#include <QBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExpValidator>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QTextBrowser>
#include <QTextCursor>
#include <QTextBlock>
#include <QScrollBar>

#include "snapshotlistbutton.h"
#include "heaptreebutton.h"
#include "parse-massif-file/parser-massif.h"
#include "chart.h"

#include "listbutton.h"

QT_CHARTS_USE_NAMESPACE

class GeneralTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralTabWidget(QWidget *parent = nullptr,
                              std::string fileName = "");
    explicit GeneralTabWidget(QWidget *parent = nullptr,
                              QStringList* fileNames = new QStringList());

signals:

public slots:

private slots:
    void change_range();
    void easy_visibility();
private:
    void createChartView();
    void createChartBoxLayout();

    QBoxLayout *createChangeRangeLayout();
    QBoxLayout *createSnapshotListLayout();
    QBoxLayout *createCodeAndTreeTabLayout();
    QBoxLayout *createTreeLayout(SnapshotListButton* generalBtn);

    void createGraph();

    Chart* _chart;
    QChartView* _chartView;
    QBoxLayout* _chartBoxLayout;
    QBoxLayout* _treeBoxLayout;
    QLineEdit* _minLE;
    QLineEdit* _maxLE;
    QTextBrowser* _codeTextBrowser;
    ParserMassif* _parser;
    std::string _fileName;
    QRadioButton* _radioButtonTimeUnit;
    QLineSeries* _seriesSnapshotNum;
    QLineSeries* _seriesTimeUnit;
    QStringList* _fileNames;
    std::vector<ParserMassif*> _parsers;
};

#endif // GENERALTABWIDGET_H

