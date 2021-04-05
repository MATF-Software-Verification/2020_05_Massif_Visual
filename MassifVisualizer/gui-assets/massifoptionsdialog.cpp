#include "massifoptionsdialog.h"

MassifOptionsDialog::MassifOptionsDialog()
{
    this->setWindowTitle("Massif User Options");

    QBoxLayout* optionsBL = new QBoxLayout(QBoxLayout::TopToBottom);
    QLabel* infoL = new QLabel("For more info write: <i><b> valgrind --tool=massif --help </b></i> in terminal<br>");
    optionsBL->addWidget(infoL);

//    --stacks
    QButtonGroup* stacksGroup = new QButtonGroup();
    QBoxLayout* stacksBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* stacksL = new QLabel("<b>--stacks</b>");
    stacksBL->addWidget(stacksL);
    _yesStacksRB = new QRadioButton("yes");
    stacksBL->addWidget(_yesStacksRB);
    _noStacksRB = new QRadioButton("no");
    _noStacksRB->setChecked(true);
    stacksBL->addWidget(_noStacksRB);
    stacksGroup->addButton(_yesStacksRB);
    stacksGroup->addButton(_noStacksRB);

//    --alloc-fn=<name>         specify <name> as an alloc function [empty]
    QBoxLayout* allocFunBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* allocFunL = new QLabel("<b>--alloc-fn</b>");
    allocFunBL->addWidget(allocFunL);
    _allocLE = new QLineEdit();
    allocFunBL->addWidget(_allocLE);

//    --threshold=<m.n>         significance threshold, as a percentage [1.0]
    QBoxLayout* thresholdBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* thresholdL = new QLabel("<b>--threshold</b>");
    thresholdBL->addWidget(thresholdL);
    _thresholdLE = new QLineEdit();
    _thresholdLE->setValidator(new QRegExpValidator
                               (QRegExp("^(([0-9]\\.[0-9]+)|([1-9][0-9]\\.[0-9]+)|((100)|(100\\.[0]+)))$")));
    thresholdBL->addWidget(_thresholdLE);

//    --time-unit=i|ms|B        time unit: instructions executed, milliseconds
//                              or heap bytes alloc'd/dealloc'd [i]
    QButtonGroup* timeUnitGroup = new QButtonGroup();
    QBoxLayout* timeUnitBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* timeUnitL = new QLabel("<b>--time-unit</b>");
    timeUnitBL->addWidget(timeUnitL);
    _timeUnitI = new QRadioButton("i");
    _timeUnitI->setChecked(true);
    timeUnitBL->addWidget(_timeUnitI);
    _timeUnitms = new QRadioButton("ms");
    timeUnitBL->addWidget(_timeUnitms);
    _timeUnitB = new QRadioButton("B");
    timeUnitBL->addWidget(_timeUnitB);
    timeUnitGroup->addButton(_timeUnitI);
    timeUnitGroup->addButton(_timeUnitms);
    timeUnitGroup->addButton(_timeUnitB);

//    --detailed-freq=<N>       every Nth snapshot should be detailed [10]
    QBoxLayout* detailedFreqBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* detailedFreqL = new QLabel("<b>--detailed-freq</b>");
    detailedFreqBL->addWidget(detailedFreqL);
    _detailedFreqLE = new QLineEdit();
    _detailedFreqLE->setValidator(new QRegExpValidator
                               (QRegExp("^(([1-9][0-9]*)|[0])$")));
    detailedFreqBL->addWidget(_detailedFreqLE);

//    --max-snapshots=<N>       maximum number of snapshots recorded [100]
    QBoxLayout* maxSnapsBL = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel* maxSnapsL = new QLabel("<b>--max-snapshots</b>");
    maxSnapsBL->addWidget(maxSnapsL);
    _maxSnapshotsLE = new QLineEdit();
    _maxSnapshotsLE->setValidator(new QRegExpValidator
                               (QRegExp("^(([1-9][0-9]*)|[0])$")));
    maxSnapsBL->addWidget(_maxSnapshotsLE);

//   THIS OPTION IS NOT SUPPORTED!!!
//    --massif-out-file=<file>  output file name [massif.out.%p]

    QPushButton* submitB = new QPushButton("Submit");
    QObject::connect(submitB, SIGNAL(clicked()), this, SLOT(submit_massif_options()));

    optionsBL->addLayout(stacksBL);
    optionsBL->addLayout(allocFunBL);
    optionsBL->addLayout(thresholdBL);
    optionsBL->addLayout(timeUnitBL);
    optionsBL->addLayout(detailedFreqBL);
    optionsBL->addLayout(maxSnapsBL);
    optionsBL->addWidget(submitB);
    this->setLayout(optionsBL);
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

    this->close();
}

QString MassifOptionsDialog::massifOptions() const
{
    return _massifOptions;
}