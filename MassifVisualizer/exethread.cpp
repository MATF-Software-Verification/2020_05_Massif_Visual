#include "exethread.h"

ExeThread::ExeThread(QObject *parent, QString exeFilePath, ConfigDialog* configDialog, MassifOptionsDialog* optionsDialog) :
    QThread(parent), _exeFilePath(exeFilePath), _configDialog(configDialog), _optionsDialog(optionsDialog)
{

}

void ExeThread::run()
{
    QProcess *valgrindMassifProcess = new QProcess();
    QString workingDirectory = _exeFilePath.mid(0, _exeFilePath.lastIndexOf("/")+1);
    valgrindMassifProcess->setWorkingDirectory(workingDirectory);

    QString valgrindPath = "valgrind";

    if (_configDialog!=nullptr && _configDialog->valgrindPath().size() > 0) {
        valgrindPath = _configDialog->valgrindPath();
    }

    QString massifOptions = "";
    if (_optionsDialog!=nullptr && _optionsDialog->massifOptions().size() > 0)
        massifOptions = _optionsDialog->massifOptions();

    std::cout << "B " << std::endl;

    QString processCommand = valgrindPath + " --tool=massif " + massifOptions + _exeFilePath;
    std::cout << "processCommand : " << processCommand.toStdString() << std::endl;
    valgrindMassifProcess->start(processCommand);
    QString massifFileName = workingDirectory + "/massif.out." + QString::number(valgrindMassifProcess->processId());

    valgrindMassifProcess->waitForFinished();

    emit valgrindMassifFinished(massifFileName);
}
