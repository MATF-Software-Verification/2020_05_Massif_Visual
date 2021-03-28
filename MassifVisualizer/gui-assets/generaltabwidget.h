#ifndef GENERALTABWIDGET_H
#define GENERALTABWIDGET_H

#include <fstream>

#include <QWidget>
#include <QChartView>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QLineSeries>
#include <QValueAxis>
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

QT_CHARTS_USE_NAMESPACE

class GeneralTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralTabWidget(QWidget *parent = nullptr,
                              std::string fileName = "");

signals:

public slots:

private slots:

    void changeRange();
    void open_and_jump_code_file();
    void showTimeUnitGraph();

    void create_my_children();
    void create_tree_root();

private:

    void createChart();
    void createChartView();
    void createChartBoxLayout();

    QBoxLayout *createChangeRangeLayout();
    QBoxLayout *createSnapshotListLayout();
    QBoxLayout *createCodeAndTreeTabLayout();
    QBoxLayout *createTreeButtonsLayout();

    void createGraph();

    QChart* _chart;
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

    void highlightLine(unsigned lineNumber);
};

#endif // GENERALTABWIDGET_H

