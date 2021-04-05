#ifndef MASSIFOPTIONSDIALOG_H
#define MASSIFOPTIONSDIALOG_H

#include <QObject>
#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QRegExpValidator>
#include "../helper-functions.h"

// TODO: remove this when not needed
#include <iostream>

class MassifOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    MassifOptionsDialog();
    ~MassifOptionsDialog();

    QString massifOptions() const;

private slots:
    void submit_massif_options();

private:
    QString _massifOptions="";

    QRadioButton* _yesStacksRB=nullptr;
    QRadioButton* _noStacksRB=nullptr;
    QLineEdit* _allocLE=nullptr;
    QLineEdit* _thresholdLE=nullptr;
    QRadioButton* _timeUnitI=nullptr;
    QRadioButton* _timeUnitms=nullptr;
    QRadioButton* _timeUnitB=nullptr;
    QLineEdit* _detailedFreqLE=nullptr;
    QLineEdit* _maxSnapshotsLE=nullptr;

    QBoxLayout* _optionsBL;
    QButtonGroup* _stacksGroup;
    QButtonGroup* _timeUnitGroup;
};

#endif // MASSIFOPTIONSDIALOG_H
