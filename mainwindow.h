#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "fclientupdater.h"
#include <QDebug>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void refreshValues();
    void consoleOut(QString s);

private slots:
    void on_updateButton_clicked();

    void on_restoreButton_clicked();

    void on_refreshButton_clicked();

    void on_toggleConsole_clicked();

    void on_osSelect_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
    FClientUpdater *cUpdater = new FClientUpdater();
    int chosenOs; // 1 : Linux | 2 : Windows
};

#endif // MAINWINDOW_H
