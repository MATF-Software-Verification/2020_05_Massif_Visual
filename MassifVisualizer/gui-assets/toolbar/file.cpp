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

void MainWindow::onValgrindMassifFinished(QString massifOutputName, int duration, int error)
{
    visualizeData(massifOutputName);
    if(error != 0){
        _msgBox.setWindowTitle("Executed");

        QString msgText = "Error: ";
        msgText.append(QString::number(error));
        msgText.append("\n");
        msgText.append("Duration: ");
        msgText.append(QString::number(duration));

        _msgBox.setText(msgText);
    }else {
        _msgBox.close();
    }
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

    std::vector<ParserMassif*> parsers;
    for(QString fileName : *_fileNamesPtr){
        ParserMassif* parser = new ParserMassif(fileName.toStdString());
        parser->parseMassifOutput();

        if (parser->validateMassifFile())
            parsers.push_back(parser);
        else {
            QString title = "Invalid Massif Input File " + QString::fromUtf8("\xF0\x9F\x90\x92");
            QString text = "Invalid massif output file: " + fileName;
            createMessageBox(title, text);
        }
    }

    if (parsers.empty()) {
        QString title = "Invalid Massif Input Files " + QString::fromUtf8("\xF0\x9F\x90\x92");
        QString text = "All massif.out files are invalid.\n"
                       "They must have a format as shown in our input-examples directory.";
        createMessageBox(title, text);
    }
    else {
        GeneralTabWidget* tmp = new GeneralTabWidget(this, _fileNamesPtr, parsers);
        int indexxx = ui->tabWidget->addTab(tmp, "Multiple Graphs");
        ui->tabWidget->setCurrentIndex(indexxx);
    }
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
    for(int i = 0; i < _recentFileActionList.size() ; i++){
        _recentFileActionList[i]->setVisible(false);
    }
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

    ParserMassif* parser = new ParserMassif(_fileName);
    parser->parseMassifOutput();

    if (!parser->validateMassifFile()) {
        QString title = "Invalid Massif Input File " + QString::fromUtf8("\xF0\x9F\x90\x92");
        QString text = "Check the validity of your massif.out file. \n"
                       "It must have a format as shown in our input-examples directory.";
        createMessageBox(title, text);
    }
    else {
        int indexxx = ui->tabWidget->addTab(new GeneralTabWidget(this, _fileName, parser), QString::fromStdString(_fileName).mid(indexx+1));
        ui->tabWidget->setCurrentIndex(indexxx);
    }
}

void MainWindow::runMassif(QString exeFileName)
{
    _exeThread = new ExeThread(this, exeFileName, _dialogPath, _dialogMOptions);
    connect(_exeThread, SIGNAL(valgrindMassifFinished(QString, int, int)),
                this, SLOT(onValgrindMassifFinished(QString, int, int)));
    _exeThread->start();
    _msgBox.setText("Executing...");
    _msgBox.setWindowTitle("In progress...");
    setTheme(qobject_cast<QWidget*>(&_msgBox));
    _msgBox.exec();
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
        recentFile->setVisible(false);
    }
    parseRecentFiles();
    updateMenus();
}

void MainWindow::updateMenus()
{
    for(int i = 0; i < _recentFileActionList.size() ; i++){
        if(QString::fromStdString(_recentFiles[static_cast<unsigned long>(i)]) == ""){
            continue;
        }
        _recentFileActionList[i]->setText(QString::fromStdString(_recentFiles[static_cast<unsigned long>(i)]));
        _recentFileActionList[i]->setData(QString::fromStdString(_recentFiles[static_cast<unsigned long>(i)]));
        _recentFileActionList[i]->setVisible(true);
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
