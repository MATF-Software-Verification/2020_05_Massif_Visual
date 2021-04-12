#include "../../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Help me box " + QString::fromUtf8("\xF0\x9F\x8F\xA5"));
    msgBox.setText("You are on your own.");
    setTheme(qobject_cast<QWidget*>(&msgBox));
    msgBox.exec();
}

void MainWindow::on_actionReport_Bug_triggered()
{
    std::string title = "So you wanted to report a bug? "  + QString::fromUtf8("\xF0\x9F\x90\xBF").toStdString();
    std::string description = "Describe in detail the bug that you have encountered... "
                   "Your description matters to us! "
                   "The more you write the more effective we will be in solving the bug! ";
    std::string input = "Are you sure you've really found a bug?";

    bool ok;
    QString text = QInputDialog::getText(this, tr(title.c_str()),
                                             tr(description.c_str()), QLineEdit::Normal,
                                             tr(input.c_str()), &ok);
    if (ok && !text.isEmpty()){
        _movie = new QMovie("../MassifVisualizer/assets/trash.gif");
        _processLabel = new QLabel(this);
        if(_wdg && _wdg->isVisible())
            return;
        _wdg = new QWidget;

        _processLabel->setParent(_wdg);
        _processLabel->setFixedSize(700, 300);
        _processLabel->setMovie(_movie);
        _wdg->setWindowTitle("Your bug report is being processed... " + QString::fromUtf8("\xF0\x9F\x93\x9C"));
        _wdg->show();

        _movie->start();
    }
}
