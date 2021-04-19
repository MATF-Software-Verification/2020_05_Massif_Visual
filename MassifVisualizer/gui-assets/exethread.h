#ifndef EXETHREAD_H
#define EXETHREAD_H

#include <QThread>
#include <QProcess>
#include <iostream>

#include "./gui-assets/configdialog.h"
#include "./gui-assets/massifoptionsdialog.h"

class ExeThread : public QThread
{
    Q_OBJECT
public:
    ExeThread(QObject *parent=nullptr,
              QString exeFilePath="",
              ConfigDialog* configDialog=nullptr,
              MassifOptionsDialog* optionsDialog=nullptr);

    ~ExeThread();

    void run();

private:
    QString _exeFilePath;
    QProcess* _valgrindMassifProcess;
    ConfigDialog* _configDialog;
    MassifOptionsDialog* _optionsDialog;
    int _duration;

signals:
    void valgrindMassifFinished(QString, int, int);
};

#endif // EXETHREAD_H
