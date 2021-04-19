#include "exethread.h"
#include <chrono>
#include <ctime>
ExeThread::ExeThread(QObject *parent, QString exeFilePath, ConfigDialog* configDialog, MassifOptionsDialog* optionsDialog) :
    QThread(parent), _exeFilePath(exeFilePath), _configDialog(configDialog), _optionsDialog(optionsDialog)
{

}

ExeThread::~ExeThread()
{
    delete _valgrindMassifProcess;
}

void ExeThread::run()
{
    _valgrindMassifProcess = new QProcess();
    QString workingDirectory = _exeFilePath.mid(0, _exeFilePath.lastIndexOf("/")+1);
    _valgrindMassifProcess->setWorkingDirectory(workingDirectory);

    QString valgrindPath = "valgrind";

    if (_configDialog!=nullptr && _configDialog->valgrindPath().size() > 0) {
        valgrindPath = _configDialog->valgrindPath();
    }

    QString massifOptions = "";
    if (_optionsDialog!=nullptr && _optionsDialog->massifOptions().size() > 0)
        massifOptions = _optionsDialog->massifOptions();

    QString processCommand = valgrindPath + " --tool=massif " + massifOptions + _exeFilePath;
    _valgrindMassifProcess->start(processCommand);
    auto startTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    QString massifFileName = workingDirectory + "/massif.out." + QString::number(_valgrindMassifProcess->processId());
    _valgrindMassifProcess->waitForFinished();
    auto exitTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    _duration = static_cast<int>(exitTime - startTime);

    emit valgrindMassifFinished(massifFileName, _duration, _valgrindMassifProcess->exitCode());
}
