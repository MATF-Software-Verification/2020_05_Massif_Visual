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

    QString labelStyle = "";
    QString lineEditStyle = "";
    QString backgroundStyle = "";
    QString textStyle = "";
    QString btnStyle = "";
    QString btnPressed = "";
    switch(theme){
        case Theme::DEFAULT:
            labelStyle = "QLabel {color: #ff8000; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #262626}\n";
            backgroundStyle = "* {background-color: #4d4d4d}\n";
            textStyle = "* {color : white}\n";
            btnStyle = "QPushButton { background-color: #ff8000; border: 1px solid #ff8000; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}";
            this->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
        case Theme::BRIGHT:
            labelStyle = "QLabel {color: black; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #ffe6cc}\n";
            backgroundStyle = "* {background-color: #f2f2f2}\n";
            textStyle = "* {color : black}\n";
            btnStyle = "QPushButton { background-color: #ffe6cc; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}";
            this->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
        case Theme::PSYCHEDELIC:
            labelStyle = "QLabel {color: #0033cc; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #993399}\n";
            backgroundStyle = "* {background-color: #4dffa6}\n";
            textStyle = "* {color : #26004d}\n";
            btnStyle = "QPushButton { background-color: #ffe6cc; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color: #ffbf80; border: 1px solid black;}";
            this->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
        case Theme::SAPPHIRE:
            labelStyle = "QLabel {color: #e6f3ff; font-size: 12pt; font-weight: bold}\n";
            lineEditStyle = "QLineEdit {background-color: #cce6ff; color: #004080}\n";
            backgroundStyle = "* {background-color: #004080}\n";
            textStyle = "* {color : #e6f3ff}\n";
            btnStyle = "QPushButton { background-color: #4db8ff; border: 1px solid black; border-radius: 5px; color: black; padding: 3px; font-size: 12pt; font-weight: bold}\n";
            btnPressed = "QPushButton:pressed { background-color:  #ccebff; border: 1px solid black;}";
            this->setStyleSheet(labelStyle + lineEditStyle + backgroundStyle + textStyle + btnStyle + btnPressed);
            break;
    }
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
