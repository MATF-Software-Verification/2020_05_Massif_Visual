#include "massifoptionsdialog.h"

#include <iostream>

MassifOptionsDialog::MassifOptionsDialog()
{
    this->setWindowTitle("Massif User Options " + QString::fromUtf8("\xF0\x9F\x90\x89"));

    _optionsBL = new QBoxLayout(QBoxLayout::TopToBottom);
    QLabel* infoL = new QLabel("For more info write: <i><b> valgrind --tool=massif --help </b></i> in terminal<br>");
    _optionsBL->addWidget(infoL);

//    --stacks
    _stacksGroup = new QButtonGroup();
    QBoxLayout* stacksBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* stacksL = new QLabel("<pre><b>--stacks       </b></pre>");
    stacksBL->addWidget(stacksL);
    _yesStacksRB = new QRadioButton("yes");
    stacksBL->addWidget(_yesStacksRB);
    _noStacksRB = new QRadioButton("no");
    _noStacksRB->setChecked(true);
    stacksBL->addWidget(_noStacksRB);
    _stacksGroup->addButton(_yesStacksRB);
    _stacksGroup->addButton(_noStacksRB);

//    --alloc-fn=<name>         specify <name> as an alloc function [empty]
    QBoxLayout* allocFunBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* allocFunL = new QLabel("<pre><b>--alloc-fn     </b></pre>");
    allocFunBL->addWidget(allocFunL);
    _allocLE = new QLineEdit();
    allocFunBL->addWidget(_allocLE);

//    --threshold=<m.n>         significance threshold, as a percentage [1.0]
    QBoxLayout* thresholdBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* thresholdL = new QLabel("<pre><b>--threshold    </b></pre>");
    thresholdBL->addWidget(thresholdL);
    _thresholdLE = new QLineEdit();
    _thresholdLE->setValidator(new QRegExpValidator
                               (QRegExp("^(([0-9]\\.[0-9]+)|([1-9][0-9]\\.[0-9]+)|((100)|(100\\.[0]+)))$")));
    thresholdBL->addWidget(_thresholdLE);

//    --time-unit=i|ms|B        time unit: instructions executed, milliseconds
//                              or heap bytes alloc'd/dealloc'd [i]
    _timeUnitGroup = new QButtonGroup();
    QBoxLayout* timeUnitBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* timeUnitL = new QLabel("<pre><b>--time-unit    </b></pre>");
    timeUnitBL->addWidget(timeUnitL);
    _timeUnitI = new QRadioButton("i");
    _timeUnitI->setChecked(true);
    timeUnitBL->addWidget(_timeUnitI);
    _timeUnitms = new QRadioButton("ms");
    timeUnitBL->addWidget(_timeUnitms);
    _timeUnitB = new QRadioButton("B");
    timeUnitBL->addWidget(_timeUnitB);
    _timeUnitGroup->addButton(_timeUnitI);
    _timeUnitGroup->addButton(_timeUnitms);
    _timeUnitGroup->addButton(_timeUnitB);

//    --detailed-freq=<N>       every Nth snapshot should be detailed [10]
    QBoxLayout* detailedFreqBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* detailedFreqL = new QLabel("<pre><b>--detailed-freq</b></pre>");
    detailedFreqBL->addWidget(detailedFreqL);
    _detailedFreqLE = new QLineEdit();
    _detailedFreqLE->setValidator(new QRegExpValidator
                               (QRegExp("^(([1-9][0-9]*)|[0])$")));
    detailedFreqBL->addWidget(_detailedFreqLE);

//    --max-snapshots=<N>       maximum number of snapshots recorded [100]
    QBoxLayout* maxSnapsBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* maxSnapsL = new QLabel("<pre><b>--max-snapshots</b></pre>");
    maxSnapsBL->addWidget(maxSnapsL);
    _maxSnapshotsLE = new QLineEdit();
    _maxSnapshotsLE->setValidator(new QRegExpValidator
                               (QRegExp("^(([1-9][0-9]*)|[0])$")));
    maxSnapsBL->addWidget(_maxSnapshotsLE);

//   THIS OPTION IS NOT SUPPORTED!!!
//    --massif-out-file=<file>  output file name [massif.out.%p]

    QBoxLayout* claBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* argInfoL = new QLabel("\nOptions for executable file\n");
    QLabel* claL = new QLabel("<pre><b>command line arguments: </b></pre>");
    claBL->addWidget(claL);
    _claLE = new QLineEdit();
    claBL->addWidget(_claLE);

    QBoxLayout* buttonBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QPushButton* resetB = new QPushButton("Reset");
    QObject::connect(resetB, SIGNAL(clicked()), this, SLOT(reset_to_default()));
    buttonBL->addWidget(resetB);

    QPushButton* submitB = new QPushButton("Submit");
    QObject::connect(submitB, SIGNAL(clicked()), this, SLOT(submit_massif_options()));
    buttonBL->addWidget(submitB);

    _optionsBL->addLayout(stacksBL);
    _optionsBL->addLayout(allocFunBL);
    _optionsBL->addLayout(thresholdBL);
    _optionsBL->addLayout(timeUnitBL);
    _optionsBL->addLayout(detailedFreqBL);
    _optionsBL->addLayout(maxSnapsBL);
    _optionsBL->addWidget(argInfoL);
    _optionsBL->addLayout(claBL);
    _optionsBL->addLayout(buttonBL);
    this->setLayout(_optionsBL);

    setTheme(qobject_cast<QWidget*>(this));
}

MassifOptionsDialog::~MassifOptionsDialog()
{
    delete _optionsBL;
    delete _stacksGroup;
    delete _timeUnitGroup;
}

void MassifOptionsDialog::submit_massif_options()
{
    if (_yesStacksRB->isChecked()) {
        _massifOptions.append("--stacks=yes ");
    }

    if (_allocLE->text().size() > 0) {
        _massifOptions.append("--alloc-fn=" + _allocLE->text() + " ");
    }

    if (_thresholdLE->text().size() > 0) {
        _massifOptions.append("--threshold=" + _thresholdLE->text() + " ");
    }

    if (_timeUnitms->isChecked()) {
        _massifOptions.append("--time-unit=ms ");
    }
    else if (_timeUnitB->isChecked()){
        _massifOptions.append("--time-unit=B ");
    }

    if (_detailedFreqLE->text().size() > 0) {
        _massifOptions.append("--detailed-freq=" + _detailedFreqLE->text() + " ");
    }

    if (_maxSnapshotsLE->text().size() > 0) {
        _massifOptions.append("--max-snapshots=" + _maxSnapshotsLE->text() + " ");
    }

    if (_claLE->text().size() > 0) {
        _commandLineArguments = _claLE->text();
        std::cout << _commandLineArguments.toStdString() << std::endl;
    }

    this->close();
}

void MassifOptionsDialog::reset_to_default()
{
    _yesStacksRB->setChecked(false);
    _noStacksRB->setChecked(false);
    _allocLE->clear();
    _thresholdLE->clear();
    _timeUnitms->setChecked(false);
    _timeUnitB->setChecked(false);
    _timeUnitI->setChecked(false);
    _detailedFreqLE->clear();
    _maxSnapshotsLE->clear();
    _claLE->clear();

    _massifOptions = "";
    _commandLineArguments = "";
}

QString MassifOptionsDialog::commandLineArguments() const
{
    return _commandLineArguments;
}

QString MassifOptionsDialog::massifOptions() const
{
    return _massifOptions;
}
