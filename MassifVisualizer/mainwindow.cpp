#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>

//needed for reading from a file
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    std::cout << _fileName << std::endl;

    //TODO: parse the input

    //template for reading from a file
   /* std::ifstream in(fileName.toStdString());
    char c;
    while (in >> c) {
        std::cout << c;
    }
    std::cout << std::endl;*/


    //TODO: visualize the data
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
}
