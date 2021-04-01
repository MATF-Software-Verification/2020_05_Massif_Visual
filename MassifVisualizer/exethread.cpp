#include "exethread.h"

ExeThread::ExeThread(QObject *parent, QString exeFilePath) :
    QThread(parent), _exeFilePath(exeFilePath)
{

}

void ExeThread::run()
{
    QProcess *valgrindMassifProcess = new QProcess();
    QString processCommand = "valgrind --tool=massif " + _exeFilePath;
    valgrindMassifProcess->start(processCommand);
    QString massifFileName = "massif.out." + QString::number(valgrindMassifProcess->processId());
    valgrindMassifProcess->waitForFinished();

    emit valgrindMassifFinished(massifFileName);
}
