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
#include "../themestyles.h"

class MassifOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    MassifOptionsDialog();
    ~MassifOptionsDialog();

    QString massifOptions() const;
    QString commandLineArguments() const;

private slots:
    void submit_massif_options();
    void reset_to_default();

private:
    QString _massifOptions="";
    QString _commandLineArguments="";

    QRadioButton* _yesStacksRB=nullptr;
    QRadioButton* _noStacksRB=nullptr;
    QLineEdit* _allocLE=nullptr;
    QLineEdit* _thresholdLE=nullptr;
    QRadioButton* _timeUnitI=nullptr;
    QRadioButton* _timeUnitms=nullptr;
    QRadioButton* _timeUnitB=nullptr;
    QLineEdit* _detailedFreqLE=nullptr;
    QLineEdit* _maxSnapshotsLE=nullptr;
    QLineEdit* _claLE=nullptr;

    QBoxLayout* _optionsBL;
    QButtonGroup* _stacksGroup;
    QButtonGroup* _timeUnitGroup;
};

#endif // MASSIFOPTIONSDIALOG_H
