#include "../../mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_actionOpen_recent_triggered()
{
    ui->menuFile->addMenu(_recentFilesMenu);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
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

    GeneralTabWidget* tmp = new GeneralTabWidget(this, _fileNamesPtr);

    int indexxx = ui->tabWidget->addTab(tmp, "Multiple Graphs");

    ui->tabWidget->setCurrentIndex(indexxx);
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
