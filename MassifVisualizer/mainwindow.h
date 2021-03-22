#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QList>

#include "gui-assets/generaltabwidget.h"

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

    void on_actionOpen_Massif_File_triggered();
    void on_actionHelp_triggered();
    void on_actionOpen_recent_triggered();
    void openRecent();
    void clearRecent();
    void quit();
    void on_tabWidget_tabCloseRequested(int index);
    void on_actionReport_Bug_triggered();
private:

    Ui::MainWindow *ui;
    std::string _fileName;
    std::string _directoryName;
    std::string _codeFileName;
    unsigned long _numRecent = 2;
    QMenu* recentFilesMenu;
    std::vector<std::string> _recentFiles;
    std::string _recentFilesFile = "../MassifVisualizer/assets/recentFiles.txt";
    QList<QAction*> _recentFileActionList;

    void visualizeData(QString fileName);
    void createMenus();
    void updateMenus();
    void parseRecentFiles();
};

#endif // MAINWINDOW_H

//how to change theme
//ui->mainToolBar->setStyleSheet("* {background-color: blue}");
//ui->centralWidget->setStyleSheet("* {background-color: blue}");
