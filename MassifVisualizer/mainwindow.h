#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionQuit_triggered();

    void on_actionOpen_Massif_File_triggered();

    void on_actionOpen_Code_File_triggered();

private:
    Ui::MainWindow *ui;
    std::string _fileName;
    std::string _codeFileName;
};

#endif // MAINWINDOW_H
