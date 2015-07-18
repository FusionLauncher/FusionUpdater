#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "fclientupdater.h"
#include <QDebug>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDataStream>
#include <QDir>

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
    void downloadLinuxClient();
    void downloadWindowsClient();

private slots:

    void updateProgressBar(qint64 current, qint64 total);

    void on_updateButton_clicked();

    void on_restoreButton_clicked();

    void on_refreshButton_clicked();

    void on_toggleConsole_clicked();

    void on_osSelect_activated(const QString &arg1);

public slots:

    void linuxFinished(QNetworkReply *reply);
    void windowsFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    FClientUpdater *cUpdater = new FClientUpdater();
    int chosenOs; // 1 : Linux | 2 : Windows
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString clientLinuxDirectory = QDir::currentPath() + "/FusionClient";
    QString clientWindowsDirectory = QDir::currentPath() + "/FusionClient.exe";
    QString clientLinuxUrl = "http://70.72.248.199/Resources/FusionClient";
    QString clientWindowsUrl = "http://70.72.248.199/Resources/FusionClient.exe";

};

#endif // MAINWINDOW_H
