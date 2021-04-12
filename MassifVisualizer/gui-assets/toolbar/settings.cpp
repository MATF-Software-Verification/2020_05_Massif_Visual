#include "../../mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::updateTreeThemes()
{
    auto n = ui->tabWidget->count();
    for(int i = 0; i < n; i++){
        GeneralTabWidget* tab = qobject_cast<GeneralTabWidget * >(ui->tabWidget->widget(i));
        for(TreeWidget* trW : tab->treeWidgets())
            trW->updateBtnTheme();
    }
}

void MainWindow::setDefaultTheme()
{
    theme = Theme::DEFAULT;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionDefault_triggered()
{
    setDefaultTheme();
}

void MainWindow::on_actionBright_theme_triggered()
{
    theme = Theme::BRIGHT;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionPsychedelic_theme_triggered()
{
    theme = Theme::PSYCHEDELIC;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionSapphire_triggered()
{
    theme = Theme::SAPPHIRE;
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionCorn_theme_triggered()
{
    theme = Theme::CORN;
    this->setWindowTitle("MassifCorn " + QString::fromUtf8("\xF0\x9F\x8C\xBD") + QString::fromUtf8("\xF0\x9F\x8C\xBD") + QString::fromUtf8("\xF0\x9F\x8C\xBD"));
    setTheme(this);
    if(_dialogPath)
        setTheme(qobject_cast<QWidget*>(_dialogPath));
    if(_dialogMOptions)
        setTheme(qobject_cast<QWidget*>(_dialogMOptions));
    updateTreeThemes();
}

void MainWindow::on_actionValgrind_Path_Config_triggered()
{
    if(_dialogPath && _dialogPath->isVisible())
        return;
    _dialogPath = new ConfigDialog();
    _dialogPath->open();
}

void MainWindow::on_actionMassif_User_Options_triggered()
{
    if(_dialogMOptions && _dialogMOptions->isVisible())
        return;
    _dialogMOptions = new MassifOptionsDialog();
    _dialogMOptions->open();
}
