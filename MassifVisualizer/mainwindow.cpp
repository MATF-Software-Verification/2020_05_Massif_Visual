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

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                              "Massif Out File (rainbow emoji)", "./", "massif.out.*");

    if (fileName.isEmpty())
        return;

    _fileName = fileName.toStdString();

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

void MainWindow::on_actionQuit_triggered()
{
     QApplication::quit();
}
