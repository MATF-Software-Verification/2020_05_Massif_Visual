#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>
#include <QTextCursor>
#include <QTextBlock>


//needed for reading from a file
#include <fstream>
#include <math.h>

#include <QMessageBox>

//koliko ovoga nam treba?
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QXYSeries>

#include <QTextBrowser>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QRegExpValidator>

QT_CHARTS_USE_NAMESPACE


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    recentFilesMenu = ui->menuFile->addMenu("Open Recent");
    parseRecentFiles();
    createMenus();

    ui->menuFile->addSeparator();

    QAction* quit = new QAction(this);
    QObject::connect(quit, SIGNAL(triggered()), this, SLOT(quit()));
    quit->setText(QString::fromStdString("Quit"));
    ui->menuFile->addAction(quit);

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPointClick(){
    auto serie = qobject_cast<QLineSeries *>(sender());
    std::cout << serie->points().first().rx() << std::endl;
}

void MainWindow::quit()
{
     QApplication::quit();
}

void MainWindow::on_actionOpen_Massif_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                              "Massif Out File (rainbow emoji)", "./", "massif.out.*");

    if (fileName.isEmpty())
        return;

    _fileName = fileName.toStdString();
    auto index = _fileName.find_last_of('/');
    _directoryName = _fileName.substr(0, (index+1));

    //TODO: parse the input

    std::ofstream outfile;
    outfile.open(_recentFilesFile, std::ios_base::app);
    outfile << _fileName << std::endl;
    outfile.close();
    parseRecentFiles();
    updateMenus();

    //TODO: visualize the data
    createGraph();

}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Help me Obi Van");
    msgBox.setText("You are on your own.");
    msgBox.exec();
}

void MainWindow::on_actionOpen_recent_triggered()
{
    ui->menuFile->addMenu(recentFilesMenu);
}

void MainWindow::openRecent()
{
    QAction *action = qobject_cast<QAction * >(sender());
    if(action){
        std::cout << action->data().toString().toStdString() << std::endl;
    }
}

void MainWindow::clearRecent(){
    std::ofstream outfile;
    outfile.open(_recentFilesFile, std::ofstream::out | std::ofstream::trunc);
    outfile.close();
    parseRecentFiles();
    updateMenus();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void MainWindow::open_and_jump_code_file()
{
    QPushButton *button= qobject_cast<QPushButton * >(sender());

    std::string fileName = button->text().split("#")[0].toStdString();
    int jumpLine = std::atoi(button->text().split("#")[1].toStdString().c_str());


    if (fileName.empty())
        return;

    _codeFileName = fileName;

    std::ifstream in(fileName);

    std::string text;
    std::string line;
    while (std::getline(in, line)) {

        line.append("\n");
        text.append(line);
    }
    in.close();
    QString code = QString::fromStdString(text);
    (ui->tabWidget->widget(ui->tabWidget->currentIndex())->findChild<QTextBrowser*>("codeTB"))->setText(code);
    //ui->textBrowser->setText(code);
    highlightLine(jumpLine);
}

void MainWindow::changeRange()
{
    QWidget* tab = ui->tabWidget->widget(ui->tabWidget->currentIndex());

    tab->dumpObjectTree();

    QChartView* chartyView = tab->findChild<QChartView*>("chartyViewMcChartChart");

    int num_minLE = 0;
    int num_maxLE = 0;
    QLineEdit* minLE = tab->findChild<QLineEdit*>("minLE");
    QLineEdit* maxLE = tab->findChild<QLineEdit*>("maxLE");

    if(minLE){

        num_minLE = std::atoi(minLE->text().toStdString().c_str());
        std::cout << num_minLE << std::endl;
    }

    if(maxLE){

        num_maxLE = std::atoi(maxLE->text().toStdString().c_str());
        std::cout << num_maxLE << std::endl;
    }

    if(chartyView and (num_minLE < num_maxLE)){

        chartyView->chart()->axes(Qt::Horizontal).back()->setRange(num_minLE, num_maxLE);
    }
    else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Invalid axis range change error");
        msgBox.setText("MIN X Axis needs to be less than MAX X Axis.\n To be not empty, they both need.");
        msgBox.exec();
    }

}


void MainWindow::highlightLine(int lineNumber)
{
    QTextBrowser* tb = ui->tabWidget->widget(ui->tabWidget->currentIndex())->findChild<QTextBrowser*>("codeTB");
    QTextCursor coursor(tb->document()->findBlockByLineNumber(lineNumber));
    QTextBlockFormat frmt = coursor.blockFormat();
    frmt.setBackground(QBrush(Qt::yellow));
    coursor.setBlockFormat(frmt);
    tb->setTextCursor(coursor);
}

QBoxLayout* MainWindow::createChangeRangeLayout()
{
    QBoxLayout *lineEditsLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    QLabel* minL = new QLabel("MIN X Axis:");
    lineEditsLayout->addWidget(minL);

    QLineEdit* minLE = new QLineEdit();
    minLE->setObjectName("minLE");
    minLE->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    lineEditsLayout->addWidget(minLE);

    QLabel* maxL = new QLabel("MAX X Axis:");
    lineEditsLayout->addWidget(maxL);

    QLineEdit* maxLE = new QLineEdit();
    maxLE->setObjectName("maxLE");
    maxLE->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    lineEditsLayout->addWidget(maxLE);

    QPushButton* submit = new QPushButton("submit");
    lineEditsLayout->addWidget(submit);

    QObject::connect(submit, SIGNAL(clicked()), this, SLOT(changeRange()));

    return lineEditsLayout;
}

QBoxLayout *MainWindow::createSnapshotListLayout()
{
    QBoxLayout *generalSnapshotListLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    QPushButton* generalPushButton1 = new QPushButton("button 1");
    QPushButton* generalPushButton2 = new QPushButton("button 2");

    generalPushButton1->setText("/home/student/Desktop/massif_example.c#3");

    QObject::connect(generalPushButton1, SIGNAL(clicked()), this, SLOT(open_and_jump_code_file()));


    generalSnapshotListLayout->addWidget(generalPushButton1);
    generalSnapshotListLayout->addWidget(generalPushButton2);

    return generalSnapshotListLayout;
}


void MainWindow::createMenus()
{

    QAction* clearRecentFiles = new QAction(this);
    QObject::connect(clearRecentFiles, SIGNAL(triggered()), this, SLOT(clearRecent()));
    clearRecentFiles->setText(QString::fromStdString("Clear recent files"));
    recentFilesMenu->addAction(clearRecentFiles);

    recentFilesMenu->addSeparator();

    for(unsigned long i = 0; i < _numRecent; i++){
        QAction* recentFile = new QAction(this);
        QObject::connect(recentFile, SIGNAL(triggered()), this, SLOT(openRecent()));
        recentFile->setText(QString::fromStdString(_recentFiles[i]));
        recentFile->setData(QString::fromStdString(_recentFiles[i]));
        recentFilesMenu->addAction(recentFile);
        _recentFileActionList.append(recentFile);
    }
}

void MainWindow::updateMenus()
{

    for(int i = 0; i < _recentFileActionList.size() ; i++){
        _recentFileActionList[i]->setText(QString::fromStdString(_recentFiles[static_cast<unsigned long>(i)]));
        _recentFileActionList[i]->setData(QString::fromStdString(_recentFiles[static_cast<unsigned long>(i)]));
    }

}

void MainWindow::parseRecentFiles()
{
    std::ifstream in(_recentFilesFile);

    std::vector<std::string> allRecentFiles;
    std::string line;
    while (std::getline(in, line)) {

        allRecentFiles.push_back(line);
    }

    if(allRecentFiles.size() < _numRecent){
        _recentFiles = std::vector<std::string>(allRecentFiles);
        for(unsigned long i = 0; i < _numRecent - static_cast<unsigned long>(allRecentFiles.size()); i++){
            _recentFiles.push_back("");
        }
        return;
    }

    std::vector<std::string>::const_iterator begin = allRecentFiles.end() - static_cast<long>(_numRecent);
    std::vector<std::string>::const_iterator end = allRecentFiles.end();
    _recentFiles = std::vector<std::string>(begin, end);

}

void MainWindow::createGraph()
{

    QLineSeries *series = new QLineSeries();
    series->append(0, 10);
    series->append(1, 11);
    series->append(2, 12);
    series->append(3, 100);
    series->append(4, 101);
    series->append(5, 100);
    series->append(6, 34);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    //how to change theme
    //ui->mainToolBar->setStyleSheet("* {background-color: blue}");
    //ui->centralWidget->setStyleSheet("* {background-color: blue}");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setObjectName("chartyViewMcChartChart");

    QBoxLayout *chartBoxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    chartBoxLayout->addWidget(chartView);

    chartBoxLayout->addLayout(createChangeRangeLayout());

    QGraphicsView* graphicsView = new QGraphicsView();
    graphicsView->setLayout(chartBoxLayout);
    QObject::connect(series, &QXYSeries::clicked, this, &MainWindow::onPointClick);

    QBoxLayout *generalTabLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    generalTabLayout->addWidget(graphicsView);

    generalTabLayout->addLayout(createSnapshotListLayout());

    QTextBrowser* codeTB = new QTextBrowser();
    codeTB->setObjectName("codeTB");

    generalTabLayout->addWidget(codeTB);

    QWidget* generalTabWidget = new QWidget();
    generalTabWidget->setLayout(generalTabLayout);

    int index =  QString::fromStdString(_fileName).lastIndexOf("/");
    ui->tabWidget->addTab(generalTabWidget, QString::fromStdString(_fileName).mid(index+1));

}
