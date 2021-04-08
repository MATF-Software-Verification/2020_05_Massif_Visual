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
#include <QRandomGenerator>

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

void MainWindow::quit()
{
     QApplication::quit();
}

void MainWindow::on_actionOpen_Massif_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                              "Massif Out File " + QString::fromUtf8("\xF0\x9F\x8C\x88"), "./", "massif.out.*");

    visualizeData(fileName);
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Help me box " + QString::fromUtf8("\xF0\x9F\x8F\xA5"));
    msgBox.setText("You are on your own.");
    setTheme(qobject_cast<QWidget*>(&msgBox));
    msgBox.exec();
}

void MainWindow::on_actionReport_Bug_triggered()
{

    std::string title = "So you wanted to report a bug? "  + QString::fromUtf8("\xF0\x9F\x90\xBF").toStdString();
    std::string description = "Describe in detail the bug that you have encountered... "
                   "Your description matters to us! "
                   "The more you write the more effective we will be in solving the bug! ";
    std::string input = "Are you sure you've really found a bug?";

    bool ok;
    QString text = QInputDialog::getText(this, tr(title.c_str()),
                                             tr(description.c_str()), QLineEdit::Normal,
                                             tr(input.c_str()), &ok);
    if (ok && !text.isEmpty()){
        _movie = new QMovie("../MassifVisualizer/assets/trash.gif");
        _processLabel = new QLabel(this);
        if(_wdg && _wdg->isVisible())
            return;
        _wdg = new QWidget;

        _processLabel->setParent(_wdg);
        _processLabel->setFixedSize(700, 300);
        _processLabel->setMovie(_movie);
        _wdg->setWindowTitle("Your bug report is being processed... " + QString::fromUtf8("\xF0\x9F\x93\x9C"));
        _wdg->show();

        _movie->start();
    }
}

void MainWindow::on_actionOpen_recent_triggered()
{
    ui->menuFile->addMenu(_recentFilesMenu);
}

void MainWindow::openRecent()
{
    QAction *action = qobject_cast<QAction*>(sender());
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
     QFileInfo check_file(fileName);
    if (fileName.isEmpty() || !check_file.exists())
        return;

    _fileName = fileName.toStdString();
    auto index = _fileName.find_last_of('/');
    _directoryName = _fileName.substr(0, (index+1));

    std::ofstream outfile;
    outfile.open(_recentFilesFile, std::ios_base::app);
    outfile << _fileName << std::endl;
    outfile.close();
    parseRecentFiles();
    updateMenus();

    int indexx =  QString::fromStdString(_fileName).lastIndexOf("/");

    int indexxx = ui->tabWidget->addTab(new GeneralTabWidget(this, _fileName), QString::fromStdString(_fileName).mid(indexx+1));
    ui->tabWidget->setCurrentIndex(indexxx);
}

void MainWindow::runMassif(QString exeFileName)
{
    _exeThread = new ExeThread(this, exeFileName, _dialogPath, _dialogMOptions);
    connect(_exeThread, SIGNAL(valgrindMassifFinished(QString)),
                this, SLOT(onValgrindMassifFinished(QString)));
    _exeThread->start();
}

void MainWindow::createMenus()
{
    _clearRecentFiles = new QAction(this);
    QObject::connect(_clearRecentFiles, SIGNAL(triggered()), this, SLOT(clearRecent()));
    _clearRecentFiles->setText(QString::fromStdString("Clear recent files"));
    _recentFilesMenu->addAction(_clearRecentFiles);

    _recentFilesMenu->addSeparator();

    for(unsigned long i = 0; i < _numRecent; i++){
        QAction* recentFile = new QAction(this);
        QObject::connect(recentFile, SIGNAL(triggered()), this, SLOT(openRecent()));
        recentFile->setText(QString::fromStdString(_recentFiles[i]));
        recentFile->setData(QString::fromStdString(_recentFiles[i]));
        _recentFilesMenu->addAction(recentFile);
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

void MainWindow::updateTreeThemes()
{
    auto n = ui->tabWidget->count();
    for(int i = 0; i < n; i++){
        GeneralTabWidget* tab = qobject_cast<GeneralTabWidget * >(ui->tabWidget->widget(i));
        for(TreeWidget* trW : tab->treeWidgets())
            trW->updateBtnTheme();
    }
}

void MainWindow::setDefaultTheme()
{
    theme = Theme::DEFAULT;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionDefault_triggered()
{
    setDefaultTheme();
}

void MainWindow::on_actionBright_theme_triggered()
{
    theme = Theme::BRIGHT;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionPsychedelic_theme_triggered()
{
    theme = Theme::PSYCHEDELIC;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionSapphire_triggered()
{
    theme = Theme::SAPPHIRE;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionCorn_theme_triggered()
{
    theme = Theme::CORN;
    this->setWindowTitle("MassifCorn " + QString::fromUtf8("\xF0\x9F\x8C\xBD") + QString::fromUtf8("\xF0\x9F\x8C\xBD") + QString::fromUtf8("\xF0\x9F\x8C\xBD"));
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionOpen_From_Executable_triggered()
{
    QString exeFileName = QFileDialog::getOpenFileName(this,
                              "Select an Executable File " + QString::fromUtf8("\xF0\x9F\x8F\xB0"), "./",  tr("Executables (*.out *.exe)"));

    runMassif(exeFileName);
}

void MainWindow::onValgrindMassifFinished(QString massifOutputName)
{
    visualizeData(massifOutputName);
}

void MainWindow::on_actionOpen_Multiple_Massif_Files_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          "Select Multiple massif.out Files " + QString::fromUtf8("\xF0\x9F\x8C\x88") + QString::fromUtf8("\xF0\x9F\x8C\x88"),
                                                          "./",
                                                          "massif.out.*");

    if(fileNames.size() == 0){
        return;
    }
    _fileNamesPtr = new QStringList(fileNames);

    int indexxx = ui->tabWidget->addTab(new GeneralTabWidget(this, _fileNamesPtr), "Multiple Graphs");
    ui->tabWidget->setCurrentIndex(indexxx);
}

void MainWindow::on_actionValgrind_Path_Config_triggered()
{
    if(_dialogPath && _dialogPath->isVisible())
        return;
    _dialogPath = new ConfigDialog();
    _dialogPath->open();
}

void MainWindow::on_actionMassif_User_Options_triggered()
{
    if(_dialogMOptions && _dialogMOptions->isVisible())
        return;
    _dialogMOptions = new MassifOptionsDialog();
    _dialogMOptions->open();
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


