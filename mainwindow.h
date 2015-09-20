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
    QDir *qd = new QDir();
    int chosenOs; // 1 : Linux | 2 : Windows
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString FcuDirectory = QDir::currentPath();
    QString linuxClient="/FusionClient";
    QString windowsClient = "/FusionClient.exe";
    QString linuxClientUrl = "http://projfusion.com/Resources/FusionClient";
    QString windowsClientUrl = "http://projfusion.com/Resources/FusionClient.exe";
    QString linuxOldFile = "/FusionClientL.OLD";
    QString windowsOldFile = "/FusionClientW.OLD";
    QString restoreFile = "/FusionClient.RESTORE";
    QString chosenPath;
    QString lVersionFile = "/FVersionL";
    QString  wVersionFile = "/FVersionW";
    QString lVersionOldFile = "/FVersionL.OLD";
    QString  wVersionOldFile = "/FVersionW.OLD";
    QString versionRestoreFile = "/FVersion.RESTORE";

};

#endif // MAINWINDOW_H
