#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libfusion.h"

#include "qdesktopservices.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::consoleOut("Started Fusion client updater.");
    cUpdater = new FClientUpdater();
    ui->pathText->blockSignals(true); //this will prevent the double fire of refreshValues();
    ui->pathText->setText(cUpdater->readPath());
    chosenPath = cUpdater->readPath();
    ui->pathText->blockSignals(false);
    MainWindow::refreshValues();


#ifdef __linux
    ui->cb_useNightly->setEnabled(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::checkFiles()
{
    //Client exists & Version missing
    if ((cUpdater->fileExists(chosenPath + clientExe)) && (!cUpdater->fileExists(LibFusion::getWorkingDir().absolutePath() + VersionFile)))
    {
        QMessageBox msg;
        msg.setText("Missing version file for client. Please run the client again to create a new one.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
        MainWindow::consoleOut("Missing version file for client.");
    }

    //Client missing & Version exists
    if ((!cUpdater->fileExists(chosenPath + clientExe)) && (cUpdater->fileExists(LibFusion::getWorkingDir().absolutePath() + VersionFile)))
    {
        MainWindow::consoleOut("Missing client but version file exists.");
    }
}


void MainWindow::downloadClient(QUrl clientAddr)
{

    MainWindow::consoleOut("Attempting to download client...");

    manager = new QNetworkAccessManager;
    QNetworkRequest request;

    request.setUrl(clientAddr);

    request.setRawHeader("User-Agent", "FCUpdater");

    reply = manager->get(request);
    ui->updateButton->setEnabled(false);
    ui->restoreButton->setEnabled(false);
    ui->pathText->setEnabled(false);

    QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgressBar(qint64,qint64)));

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void MainWindow::replyFinished(QNetworkReply *reply)
{

    reply->deleteLater();
    reply->ignoreSslErrors();

    if(reply->error())
    {
        MainWindow::consoleOut("[ERROR] Client download reply error.");
        qDebug() << reply->errorString();
        MainWindow::refreshValues();
        ui->progressBar->setValue(0);
        return;
    }
    else if(reply->url() != clientURL && reply->url() != nightlyClientURL && reply->url() != clientURL_FB && reply->url() != nightlyClientURL_FB)
    {

        MainWindow::consoleOut("[ERROR] Client reply URL does not match real client URL.");
        MainWindow::refreshValues();
        ui->progressBar->setValue(0);
        return;
    }
    else
    {

        QFile file;

        if(OS==Windows)
            file.setFileName(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/FusionLauncher.exe");
        else
            file.setFileName(chosenPath + clientExe);

    if(file.exists())
        file.remove();


        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();
        ui->progressBar->setMaximum(1);
        ui->progressBar->setValue(0);
        MainWindow::consoleOut("Client finished downloading.");


        if(OS==Windows) {
            bool launch = QDesktopServices::openUrl(QUrl("file:///" + file.fileName(), QUrl::TolerantMode) );
            if(launch)
                this->close();
        }


        MainWindow::refreshValues();
    }
}


void MainWindow::updateProgressBar(qint64 current, qint64 total)
{
    ui->updateButton->setEnabled(false);
    ui->restoreButton->setEnabled(false);
    ui->pathText->setEnabled(false);

    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}


void MainWindow::restoreClient()
{

    ui->pathText->setEnabled(false);


    MainWindow::consoleOut("Attempting to restore client...");

    //Rename previous client.
    qd->rename(chosenPath + clientExeOld, chosenPath + restoreFile);
    qd->rename(chosenPath + VersionOldFile, chosenPath + versionRestoreFile);

    //Rename unwanted client.
    qd->rename(chosenPath + clientExe, chosenPath + clientExeOld);
    qd->rename(chosenPath + VersionFile, chosenPath + VersionOldFile);

    //Rename previous client again.
    qd->rename(chosenPath + restoreFile, chosenPath + clientExe);
    qd->rename(chosenPath + versionRestoreFile, chosenPath + VersionFile);

    MainWindow::refreshValues();
}

void MainWindow::refreshValues()
{
    VersionCheckResult vcr_online;
    if(ui->cb_useNightly->checkState())
    {
        if(ui->cb_alternateServer->checkState())
            vcr_online = cUpdater->getCRClientVersion(nightlyVersionFile_FB);
        else
            vcr_online = cUpdater->getCRClientVersion(nightlyVersionFile);

        MainWindow::consoleOut("Getting latest nightly Version...");
    }
    else
    {
        if(ui->cb_alternateServer->checkState())
            vcr_online = cUpdater->getCRClientVersion(stableVersionFile_FB);
        else
            vcr_online = cUpdater->getCRClientVersion(stableVersionFile);

        MainWindow::consoleOut("Getting latest stable Version...");
    }

    if(vcr_online.error != "NoError"){
        consoleOut(vcr_online.error);
        return;
    }
    online = vcr_online.version;
    if(online.Build + online.Minor + online.Major == 0){
        consoleOut("Error reading version!");
        return;
    }

    ui->cVersionLabel->setText(cUpdater->VersionToStr(online));

    ui->updateButton->setEnabled(true);
    ui->pathText->setEnabled(true);

    MainWindow::checkFiles();
    MainWindow::consoleOut("Read Clientversion from " + LibFusion::getWorkingDir().absolutePath() + VersionFile);

    installed = cUpdater->getDLClientVersion(LibFusion::getWorkingDir().absolutePath() + VersionFile);
    ui->dVersionLabel->setText(cUpdater->VersionToStr(installed));

    if (cUpdater->fileExists(chosenPath + clientExe))
        ui->updateButton->setText("Update");
    else
        ui->updateButton->setText("Download");

    if (cUpdater->fileExists(chosenPath + clientExeOld))
        ui->restoreButton->setEnabled(true);
    else
        ui->restoreButton->setEnabled(false);


    MainWindow::consoleOut("Refreshing done.");
}

void MainWindow::consoleOut(QString s)
{
    qDebug() << "["+ QTime::currentTime().toString() + "] " + s;
    ui->consoleOutput->appendPlainText("[" + QTime::currentTime().toString() + "] " + s + "\n");
}

void MainWindow::on_updateButton_clicked()
{
    ui->pathText->setEnabled(false);

    if (installed==online && online.initialized)
    {
        MainWindow::consoleOut("Client does not need updated.");
    }
    else if (!online.initialized)
    {
        MainWindow::consoleOut("Couldn't get Online-Version!");
    }
    else
    {
        if(ui->cb_useNightly->checkState())
        {
            if(ui->cb_alternateServer->checkState())
                MainWindow::downloadClient(nightlyClientURL_FB);
            else
                MainWindow::downloadClient(nightlyClientURL);
        }
        else
        {
            if(ui->cb_alternateServer->checkState())
                MainWindow::downloadClient(clientURL_FB);
            else
                MainWindow::downloadClient(clientURL);
        }

        MainWindow::consoleOut("Client updated.");
    }
}

void MainWindow::on_restoreButton_clicked()
{

    ui->pathText->setEnabled(false);


    if (cUpdater->fileExists(chosenPath + clientExeOld))
    {

        MainWindow::restoreClient();
        MainWindow::refreshValues();
        MainWindow::consoleOut("Old client restored.");
        MainWindow::refreshValues();
    }
    else
    {
        MainWindow::consoleOut("No old client.");
        MainWindow::refreshValues();
        return;
    }


}

void MainWindow::on_refreshButton_clicked()
{
    MainWindow::refreshValues();
}


void MainWindow::on_browseButton_clicked()
{

    ui->pathText->setText(QFileDialog::getExistingDirectory(this, tr("Choose client directory."), QDir::currentPath()));
}

void MainWindow::on_pathText_textChanged(const QString &arg1)
{
    if ((arg1.isEmpty()) || (arg1.isNull()) )
    {

        ui->pathText->setText(FcuDirectory);
        chosenPath = FcuDirectory;
        MainWindow::refreshValues();
        return;
    }

    chosenPath = arg1;
    MainWindow::consoleOut("Set directory to " + chosenPath);
    MainWindow::refreshValues();
}
