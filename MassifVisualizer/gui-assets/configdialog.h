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

#include "../helper-functions.h"

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigDialog();

    QString valgrindPath() const;

private slots:
    void submit_valgrind_path();
    void enable_submit(const QString & text);

private:
    bool pathExistsAndExecutable(QString path);

    QString _valgrindPath = "valgrind";
    QLineEdit* _valgrindPathLE = nullptr;
    QPushButton* _submit = nullptr;
};

#endif // CONFIGDIALOG_H
