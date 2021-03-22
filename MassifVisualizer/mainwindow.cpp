#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <fstream>

#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QMovie>

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

void MainWindow::quit()
{
     QApplication::quit();
}

void MainWindow::on_actionOpen_Massif_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                              "Massif Out File (rainbow emoji)", "./", "massif.out.*");

    visualizeData(fileName);
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Help me Obi Van");
    msgBox.setText("You are on your own.");
    msgBox.exec();
}

void MainWindow::on_actionReport_Bug_triggered()
{

    std::string title = "So you wanted to report a bug?";
    std::string description = "Describe in detail the bug that you have ecountered... "
                   "Your description matters to us! "
                   "The more you write the more effective we will be in solving the bug! "
                   "Consider that when writing";
    std::string input = "Are you sure you've really found a bug?";

    bool ok;
    QString text = QInputDialog::getText(this, tr(title.c_str()),
                                             tr(description.c_str()), QLineEdit::Normal,
                                             tr(input.c_str()), &ok);
    if (ok && !text.isEmpty()){


        QMovie *movie = new QMovie("../MassifVisualizer/assets/bugReport.gif");
        QLabel *processLabel = new QLabel(this);
        ui->tabWidget->addTab(processLabel, "bug report procedure");
        processLabel->setMovie(movie);
        movie->start();
    }

}

void MainWindow::on_actionOpen_recent_triggered()
{
    ui->menuFile->addMenu(recentFilesMenu);
}

void MainWindow::openRecent()
{
    QAction *action = qobject_cast<QAction * >(sender());
    if(action){
        visualizeData(action->data().toString());
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

void MainWindow::visualizeData(QString fileName)
{
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
    int indexx =  QString::fromStdString(_fileName).lastIndexOf("/");

    int indexxx = ui->tabWidget->addTab(new GeneralTabWidget(), QString::fromStdString(_fileName).mid(indexx+1));
    ui->tabWidget->setCurrentIndex(indexxx);
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
