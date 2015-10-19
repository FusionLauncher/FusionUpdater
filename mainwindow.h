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

    /**
     * @brief refreshValues Will read the Version from both the Local Client and the Web-Api
     *
     */
    void refreshValues();

    /**
     * @brief consoleOut Append a String to the Console in the MainWindow
     *
     * @param s This String will be appended to the Console
     */
    void consoleOut(QString s);

    /**
     * @brief restoreClient Restore the previous version of FusionLauncher.exe
     *
     * @todo This needs to replace all Files, like the lib, CLI and Updater too.
     */
    void downloadClient();


    /**
     * @brief restoreClient Restore the previous version of FusionLauncher.exe
     *
     * @todo This needs to replace all Files, like the lib, CLI and Updater too.
     */
    void restoreClient();

    /**
     * @brief checkFiles Will check if neccessary files for Update exists
     *  - Version File local
     *  - Client File local
     *  Will Show a Message-Dialog if a file does not exists.
     */
    void checkFiles();

private slots:

    void updateProgressBar(qint64 current, qint64 total);

    void on_updateButton_clicked();

    void on_restoreButton_clicked();

    void on_refreshButton_clicked();

    void on_browseButton_clicked();

    void replyFinished(QNetworkReply *reply);

    void on_pathText_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    FClientUpdater *cUpdater;


    #ifdef _WIN32
        const supportedOS OS = Windows;
        QString clientExe = "/FusionClient.exe";
        QString clientURL = "http://projfusion.com/files/Releases/latest/FusionLauncher_Windows.exe";
        QString clientExeOld = "/FusionClientW.OLD";
    #elif __linux
        const supportedOS OS = Linux;
        QString clientExe = "/FusionClient";
        QString clientURL = "http://projfusion.com/files/Releases/latest/FusionClient_Linux.tar";
        QString clientExeOld = "/FusionClientL.OLD";
    #endif

    QString restoreFile = "/FusionClient.RESTORE";
    QString VersionFile = "/FVersion.txt";
    QString VersionOldFile = "/FVersion.OLD";
    QString versionRestoreFile = "/FVersion.RESTORE";


    QDir *qd = new QDir();
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString FcuDirectory = QDir::currentPath();
    QString chosenPath;

    FusionVersion installed;
    FusionVersion online;



};

#endif // MAINWINDOW_H
