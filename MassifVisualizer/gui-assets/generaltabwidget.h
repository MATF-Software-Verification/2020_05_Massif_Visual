#ifndef GENERALTABWIDGET_H
#define GENERALTABWIDGET_H

#include <fstream>

#include <QWidget>
#include <QChartView>
#include <QBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExpValidator>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QTextBrowser>
#include <QScrollBar>

#include "snapshotlistbutton.h"
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

    ~GeneralTabWidget();

    std::vector<TreeWidget *> treeWidgets() const;

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
    QBoxLayout *createCodeLayout();
    QBoxLayout* createPeakListLayout();

    void createGraph();

    QBoxLayout* _generalTabLayout;
    Chart* _chart;
    QChartView* _chartView;
    QBoxLayout* _chartBoxLayout;
    QBoxLayout* _treeBoxLayout;
    QLineEdit* _minLE;
    QLineEdit* _maxLE;
    QTextBrowser* _codeTextBrowser;
    ParserMassif* _parser = nullptr;
    std::string _fileName;
    std::vector<ParserMassif*> _parsers;
    std::vector<TreeWidget*> _treeWidgets;
    QStringList* _fileNames;
    QBoxLayout* _flowLayout;
    QBoxLayout* _flowAllSnapshotsLayout;
    QWidget* _scrollAreaContent;
    QWidget* _scrollAllSnapshotAreaContent;
};

#endif // GENERALTABWIDGET_H

