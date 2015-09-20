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
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

enum supportedOS {Windows, Linux};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void refreshValues();
    void consoleOut(QString s);
    void downloadClient();
    void updateClient();
    void restoreClient();
    void checkFiles();

private slots:

    void updateProgressBar(qint64 current, qint64 total);

    void on_updateButton_clicked();

    void on_restoreButton_clicked();

    void on_refreshButton_clicked();

    void on_osSelect_activated(const QString &arg1);

    void on_browseButton_clicked();

private slots:

    void replyFinished(QNetworkReply *reply);

    void on_pathText_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    FClientUpdater *cUpdater = new FClientUpdater();


    #ifdef _WIN32
        const supportedOS OS = Windows;
        QString clientExe = "/FusionClient.exe";
        QString clientURL = "http://projfusion.com/Resources/FusionClient.exe";
        QString clientExeOld = "/FusionClientW.OLD";
    #elif __linux
        const supportedOS OS = Linux;
        QString clientExe = "/FusionClient";
        QString clientURL = "http://projfusion.com/Resources/FusionClient";
        QString clientExeOld = "/FusionClientL.OLD";
    #endif

    QString restoreFile = "/FusionClient.RESTORE";
    QString VersionFile = "/FVersion";
    QString VersionOldFile = "/FVersion.OLD";
    QString versionRestoreFile = "/FVersion.RESTORE";


    QDir *qd = new QDir();
    int chosenOs; // 1 : Linux | 2 : Windows
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString FcuDirectory = QDir::currentPath();
    QString chosenPath;


};

#endif // MAINWINDOW_H
