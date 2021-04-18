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
#include "themestyles.h"

QT_CHARTS_USE_NAMESPACE

class GeneralTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralTabWidget(QWidget *parent = nullptr,
                              std::string fileName = "",
                              ParserMassif* parser = nullptr);
    explicit GeneralTabWidget(QWidget *parent = nullptr,
                              QStringList* fileNames = new QStringList(),
                              std::vector<ParserMassif*> parsers = {});

    ~GeneralTabWidget();

    std::vector<TreeWidget *> treeWidgets() const;

private slots:
    void change_range();
    void easy_visibility();

private:
    void createChartView();
    void createChartBoxLayout();

    QBoxLayout *createChangeRangeLayout();
    void initListLayout(QScrollArea *scrollArea);
    QBoxLayout *createSnapshotListLayout();
    QBoxLayout* createPeakListLayout();
    QBoxLayout *createCodeLayout();

    void createGraph();

    QBoxLayout* _generalTabLayout = nullptr;
    Chart* _chart = nullptr;
    QChartView* _chartView = nullptr;
    QBoxLayout* _chartBoxLayout = nullptr;
    QBoxLayout* _treeBoxLayout = nullptr;
    QLineEdit* _minLE = nullptr;
    QLineEdit* _maxLE = nullptr;
    QTextBrowser* _codeTextBrowser = nullptr;
    std::string _fileName;
    ParserMassif* _parser = nullptr;
    QStringList* _fileNames = nullptr;
    std::vector<ParserMassif*> _parsers;
    std::vector<TreeWidget*> _treeWidgets;
    QBoxLayout* _flowLayout = nullptr;
    QWidget* _scrollAreaContent = nullptr;
    QWidget* _scrollAllSnapshotAreaContent = nullptr;

};

#endif // GENERALTABWIDGET_H

