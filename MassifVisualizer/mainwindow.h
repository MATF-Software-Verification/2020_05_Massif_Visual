#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include <vector>

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
    void on_actionOpen_Code_File_triggered();
    void on_actionHelp_triggered();
    void on_actionOpen_recent_triggered();
    void openRecent();
    void clearRecent();
    void quit();
    void onPointClick();
    void on_tabWidget_tabCloseRequested(int index);

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
    void createMenus();
    void updateMenus();
    void parseRecentFiles();
    void createGraph();
    void highlightLine(int lineNumber);
};

#endif // MAINWINDOW_H
