#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>
#include <QTextCursor>
#include <QTextBlock>

//needed for reading from a file
#include <fstream>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuFile->addSeparator();
    recentFilesMenu = ui->menuFile->addMenu("Open Recent");
    parseRecentFiles();
    createMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
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

    //template for reading from a file
   /* std::ifstream in(fileName.toStdString());
    char c;
    while (in >> c) {
        std::cout << c;
    }
    std::cout << std::endl;*/


    //TODO: visualize the data
    //TODO: add filepath to recentFiles
}

void MainWindow::on_actionOpen_Code_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                              "Code File (code Lyoko)", "./", "*.*");

    if (fileName.isEmpty())
        return;

    _codeFileName = fileName.toStdString();

    std::ifstream in(fileName.toStdString());

    std::string text;
    std::string line;
    while (std::getline(in, line)) {

        line.append("\n");
        text.append(line);
    }
    QString text2;
    text2 = QString::fromStdString(text);
    ui->textBrowser->setText(text2);
    highlightLine(80);
}

void MainWindow::highlightLine(int lineNumber)
{
    QTextCursor coursor(ui->textBrowser->document()->findBlockByLineNumber(lineNumber));
    QTextBlockFormat frmt = coursor.blockFormat();
    frmt.setBackground(QBrush(Qt::yellow));
    coursor.setBlockFormat(frmt);
    ui->textBrowser->setTextCursor(coursor);
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox msgBox;
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

void MainWindow::createMenus()
{


    for(auto i = 0; i < _numRecent; i++){
        QAction* recentFile = new QAction(this);
        QObject::connect(recentFile, SIGNAL(triggered()), this, SLOT(openRecent()));
        recentFile->setText(QString::fromStdString(_recentFiles[i]));
        recentFile->setData(QString::fromStdString(_recentFiles[i]));
        recentFilesMenu->addAction(recentFile);
    }

}

void MainWindow::parseRecentFiles()
{
    std::ifstream in("../MassifVisualizer/assets/recentFiles.txt");

    std::string text;
    std::string line;
    while (std::getline(in, line)) {

        _recentFiles.push_back(line);
    }

}
