#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QFileDialog>

Theme theme = Theme::DEFAULT;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _recentFilesMenu = ui->menuFile->addMenu("Open Recent");
    parseRecentFiles();
    createMenus();

    ui->menuFile->addSeparator();

    _quit = new QAction(this);
    QObject::connect(_quit, SIGNAL(triggered()), this, SLOT(quit()));
    _quit->setText(QString::fromStdString("Quit"));
    ui->menuFile->addAction(_quit);

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    this->resize(1000, 600);
    this->setWindowTitle("MassifCorn " + QString::fromUtf8("\xF0\x9F\x8C\xBD"));

    setDefaultTheme();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _recentFilesMenu;
    for (QAction* qAction: _recentFileActionList)
        delete qAction;
    _recentFileActionList.clear();
    delete _dialogPath;
    delete _dialogMOptions;
    delete _quit;
    delete _movie;
    delete _processLabel;
    delete _wdg;
    delete _exeThread;
    delete _clearRecentFiles;
    delete _fileNamesPtr;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();

    if (_dialogPath != nullptr)
        _dialogPath->close();

    if (_dialogMOptions != nullptr)
        _dialogMOptions->close();

    QWidget::closeEvent(event);
    QApplication::quit();
}


