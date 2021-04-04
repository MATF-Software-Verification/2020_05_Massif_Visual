#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QObject>
#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileInfo>
#include <QMessageBox>

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigDialog();

    QString valgrindPath() const;

private slots:
    void submit_valgrind_path();

private:
    bool pathExistsAndExecutable(QString path);

    QString _valgrindPath = "valgrind";
    QLineEdit* _valgrindPathLE = nullptr;
};

#endif // CONFIGDIALOG_H
