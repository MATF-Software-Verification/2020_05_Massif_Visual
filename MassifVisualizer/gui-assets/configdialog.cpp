#include "configdialog.h"

ConfigDialog::ConfigDialog()
{
    this->setWindowTitle("Valgrind Path Config " + QString::fromUtf8("\xF0\x9F\xA6\x84"));

    _configLayoutBL = new QBoxLayout(QBoxLayout::TopToBottom);
    _insertPathL = new QLabel("Enter Valgrind System Path: ");
    _configLayoutBL->addWidget(_insertPathL);

    _pathAndButtonBL = new QBoxLayout(QBoxLayout::LeftToRight);
    _valgrindPathLE = new QLineEdit();

    _submit = new QPushButton("Submit");
    _submit->setDisabled(true);

    QObject::connect(_valgrindPathLE, SIGNAL(textChanged(QString)), this, SLOT(enable_submit(QString)));
    QObject::connect(_submit, SIGNAL(clicked()), this, SLOT(submit_valgrind_path()));

    _pathAndButtonBL->addWidget(_valgrindPathLE);
    _pathAndButtonBL->addWidget(_submit);
    _configLayoutBL->addLayout(_pathAndButtonBL);

    this->setLayout(_configLayoutBL);
    setTheme(qobject_cast<QWidget*>(this));
}

ConfigDialog::~ConfigDialog()
{
    delete _configLayoutBL;
    if (_messageBox != nullptr)
        delete _messageBox;
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
        _messageBox = new QMessageBox();
        _messageBox->setText("Wrong Valgrind Path!");
        _messageBox->setWindowTitle("Try Again");
        _messageBox->open();
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
