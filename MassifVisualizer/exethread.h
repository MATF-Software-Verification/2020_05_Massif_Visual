#ifndef EXETHREAD_H
#define EXETHREAD_H

#include <QThread>
#include <QProcess>
#include <iostream>

class ExeThread : public QThread
{
    Q_OBJECT
public:
    ExeThread(QObject *parent=nullptr, QString exeFilePath="");

    void run();
private:
    QString _exeFilePath;

signals:
    void valgrindMassifFinished(QString);
};

#endif // EXETHREAD_H
