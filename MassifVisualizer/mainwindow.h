#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <iostream>
#include <fstream>

#include <QMainWindow>
#include <QList>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QMovie>
#include <QRandomGenerator>

#include "gui-assets/generaltabwidget.h"
#include "gui-assets/exethread.h"
#include "themestyles.h"
#include "gui-assets/configdialog.h"
#include "gui-assets/massifoptionsdialog.h"

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
    void on_actionOpen_recent_triggered();
    void openRecent();
    void clearRecent();
    void on_tabWidget_tabCloseRequested(int index);
    void on_actionOpen_From_Executable_triggered();
    void onValgrindMassifFinished(QString massifOutputName, int time, int error);
    void on_actionOpen_Multiple_Massif_Files_triggered();
    void quit();

    void on_actionDefault_triggered();
    void on_actionBright_theme_triggered();
    void on_actionPsychedelic_theme_triggered();
    void on_actionSapphire_triggered();
    void on_actionCorn_theme_triggered();
    void on_actionValgrind_Path_Config_triggered();
    void on_actionMassif_User_Options_triggered();

    void on_actionHelp_triggered();
    void on_actionReport_Bug_triggered();

private:
    unsigned long _numRecent = 2;
    std::string _recentFilesFile = "../MassifVisualizer/assets/recentFiles.txt";

    Ui::MainWindow *ui;
    std::string _fileName;
    std::string _directoryName;
    std::string _codeFileName;
    QMenu* _recentFilesMenu;
    std::vector<std::string> _recentFiles;
    QList<QAction*> _recentFileActionList;
    ConfigDialog *_dialogPath = nullptr;
    MassifOptionsDialog* _dialogMOptions = nullptr;

    QAction* _quit = nullptr;
    QMovie* _movie = nullptr;
    QLabel* _processLabel = nullptr;
    QWidget* _wdg = nullptr;

    ExeThread* _exeThread = nullptr;
    QAction* _clearRecentFiles = nullptr;
    QStringList* _fileNamesPtr = nullptr;
    QMessageBox _msgBox;

    void visualizeData(QString fileName);
    void runMassif(QString exeFileName);
    void createMenus();
    void updateMenus();
    void parseRecentFiles();
    void createMessageBox(QString title, QString text);

    void setDefaultTheme();
    void updateTreeThemes();

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
