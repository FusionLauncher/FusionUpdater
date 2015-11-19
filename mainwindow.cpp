#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libfusion.h"

#include "qdesktopservices.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    consoleOut("Started Fusion client updater.");

    cUpdater = new FClientUpdater();

    ui->pathText->blockSignals(true);

    ui->pathText->setText(cUpdater->readPath());
    chosenPath = cUpdater->readPath();

    ui->pathText->blockSignals(false);


    QStringList args = qApp->arguments();


    if (args.length() == 2)
    {
        QString ar = args.value(1);
        predefinedSource = (FusionSources)ar.toInt();
        if (preDownloadCheck())
            downloadClient(predefinedSource);
    }
    else
    {
        refreshValues();
    }



#ifdef __linux
    ui->cb_useNightly->setEnabled(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::preDownloadCheck()
{
    QFile FusionExec(chosenPath + clientExe);

    if (!FusionExec.exists())
    {
        consoleOut("Missing client executable!");
    }

    ui->widget_3->setEnabled(false);
    ui->widget_2->setEnabled(false);
    ui->widget->setEnabled(false);

    return true;
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


void MainWindow::downloadClient(FusionSources source)
{
    consoleOut("Attempting to download client from");

    manager = new QNetworkAccessManager;
    QNetworkRequest request;

    switch (source) {
    case srcStable:
        request.setUrl(QUrl(UPDATER_LATES_STABLE));
        break;
     case srcStable_Alt:
         request.setUrl(QUrl(UPDATER_LATES_STABLE_ALT));
         break;
     case srcNightly:
         request.setUrl(QUrl(UPDATER_LATES_NIGHTLY));
         break;
     case srcNightly_Alt:
         request.setUrl(QUrl(UPDATER_LATES_NIGHTLY_ALT));
         break;
    default:
        return;
        break;
    }

    consoleOut(request.url().toString());
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
    else if(reply->url() != QString(UPDATER_LATES_STABLE) && reply->url() != QString(UPDATER_LATES_STABLE_ALT)
            && reply->url() != QString(UPDATER_LATES_NIGHTLY) && reply->url() != QString(UPDATER_LATES_NIGHTLY_ALT))
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
    bool Nightly = (bool)ui->cb_useNightly->checkState();
    VersionCheckResult updateStatus = cUpdater->checkForUpdate(Nightly);

    if (updateStatus.Status == ErrorOnCheckingOnline)
    {
        consoleOut("Error on checking latest version Online!");
        ui->updateButton->setEnabled( false );
        return;
    }
    else
    {
        ui->updateButton->setEnabled( true );
        ui->cVersionLabel->setText( updateStatus.VersionOnline.toString() );
    }


    if(updateStatus.Status == ErrorOnCheckingLocal)
    {
        consoleOut("Error on checking installed version!");
    }
    else
    {
        ui->dVersionLabel->setText(updateStatus.VersionLocal.toString());
    }



    predefinedSource = updateStatus.Source;
    if(updateStatus.Status == UpToDate)
    {
        consoleOut("You are up to date!");
    }
    else if ( updateStatus.Status != ErrorOnCheckingOnline && updateStatus.Status != ErrorOnCheckingLocal )
    {
        consoleOut("New Version found!\nClick Download to install.");
    }


}

void MainWindow::consoleOut(QString s)
{
    qDebug() << "["+ QTime::currentTime().toString() + "] " + s;
    ui->consoleOutput->appendPlainText("[" + QTime::currentTime().toString() + "] " + s + "\n");
}

void MainWindow::on_updateButton_clicked()
{
    ui->pathText->setEnabled(false);
    preDownloadCheck();
    downloadClient(predefinedSource);

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
