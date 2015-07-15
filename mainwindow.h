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
    void setupWindow();
    void consoleOut(QString s);

private slots:
    void on_updateButton_clicked();

    void on_restoreButton_clicked();

    void on_refreshButton_clicked();

    void on_toggleConsole_clicked();

private:
    Ui::MainWindow *ui;
    FClientUpdater *cUpdater = new FClientUpdater();
};

#endif // MAINWINDOW_H
