#include "configdialog.h"

ConfigDialog::ConfigDialog()
{
    this->setWindowTitle("Valgrind Path Config");

    QBoxLayout* configLayoutBL = new QBoxLayout(QBoxLayout::TopToBottom);
    QLabel* insertPathL = new QLabel("Enter Valgrind System Path: ");
    configLayoutBL->addWidget(insertPathL);

    QBoxLayout* pathAndButtonBL = new QBoxLayout(QBoxLayout::LeftToRight);
    _valgrindPathLE = new QLineEdit();

    _submit = new QPushButton("Submit");
    _submit->setDisabled(true);

    QObject::connect(_valgrindPathLE, SIGNAL(textChanged(QString)), this, SLOT(enable_submit(QString)));
    QObject::connect(_submit, SIGNAL(clicked()), this, SLOT(submit_valgrind_path()));

    pathAndButtonBL->addWidget(_valgrindPathLE);
    pathAndButtonBL->addWidget(_submit);
    configLayoutBL->addLayout(pathAndButtonBL);

    this->setLayout(configLayoutBL);
    setTheme(this);
}

// TODO: How to validate if it is actually valgrind exe?
bool ConfigDialog::pathExistsAndExecutable(QString path) {
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isExecutable() && check_file.isFile();
}

QString ConfigDialog::valgrindPath() const
{
    return _valgrindPath;
}

void ConfigDialog::submit_valgrind_path()
{
    QString lineEditText = _valgrindPathLE->text();

    if (pathExistsAndExecutable(lineEditText)) {
        this->close();
        _valgrindPath = lineEditText;
    }
    else {
        QMessageBox* messageBox = new QMessageBox();
        messageBox->setText("Wrong Valgrind Path!");
        messageBox->setWindowTitle("Try Again");
        messageBox->open();
        _valgrindPathLE->clear();
    }
}

void ConfigDialog::enable_submit(const QString &text)
{
    if (text.size() > 0) {
        _submit->setEnabled(true);
    }
    else {
        _submit->setDisabled(true);
    }
}
