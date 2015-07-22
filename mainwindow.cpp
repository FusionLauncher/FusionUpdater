#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::consoleOut("Started Fusion client updater.");
    ui->consoleOutput->hide();
    this->setFixedHeight(111);
    ui->osSelect->addItem("Linux");
    ui->osSelect->addItem("Windows");
    ui->pathText->setText(FcuDirectory);
    chosenOs = 1;
    chosenPath = FcuDirectory;
    MainWindow::refreshValues();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::downloadClient()
{

    manager = new QNetworkAccessManager;
    QNetworkRequest request;

    if (chosenOs == 1) { request.setUrl(linuxClientUrl); }
    else if (chosenOs == 2) { request.setUrl(windowsClientUrl); }

    reply = manager->get(request);
    ui->updateButton->setEnabled(false);
    ui->osSelect->setEnabled(false);
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
        qDebug() << "[ERROR] Client download reply error.";
        qDebug() << reply->errorString();
        ui->osSelect->setEnabled(true);
        MainWindow::refreshValues();
        ui->progressBar->setValue(0);
        return;
    }
    else if((chosenOs == 1) && (reply->url() != linuxClientUrl))
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        ui->osSelect->setEnabled(true);
        MainWindow::refreshValues();
        ui->progressBar->setValue(0);
        return;
    }
    else if((chosenOs == 2) && (reply->url() != windowsClientUrl))
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        ui->osSelect->setEnabled(true);
        MainWindow::refreshValues();
        ui->progressBar->setValue(0);
        return;
    }
    else
    {

    QByteArray ba = reply->readAll();
    QFile file;

    if (chosenOs == 1) { file.setFileName(chosenPath + linuxClient); }
    else if (chosenOs == 2) { file.setFileName(chosenPath + windowsClient); }

    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << ba;
    file.close();
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(0);
    MainWindow::consoleOut("Client finished downloading.");
    ui->osSelect->setEnabled(true);
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

//Replaces downloaded client with current client.
void MainWindow::updateClient()
{

    qDebug() << "Attempting to update client.";
    ui->pathText->setEnabled(false);

    //Rename downloaded client.
    if (chosenOs == 1)
    {

        qd->rename(chosenPath + linuxClient, chosenPath + linuxOldFile);
        qDebug() << "Renamed CURRENT to OLD";
    }
    else if (chosenOs == 2)
    {

        qd->rename(chosenPath + windowsClient, chosenPath +  windowsOldFile);
        qDebug() << "Renamed CURRENT to OLD";
    }
    MainWindow::refreshValues();
}

//Restores previous client.
void MainWindow::restoreClient()
{

    ui->pathText->setEnabled(false);

    if (chosenOs == 1)
    {

        qDebug() << "Attempting to restore Linux client.";

        //Rename previous client.
        qd->rename(chosenPath + linuxOldFile, chosenPath + restoreFile);
        qDebug() << "Renamed OLD to RESTORE.";

        //Rename unwanted client.
        qd->rename(chosenPath + linuxClient, chosenPath + linuxOldFile);
        qDebug() << "Renamed CURRENT to OLD.";

        //Rename previous client again.
        qd->rename(chosenPath + restoreFile, chosenPath + linuxClient);
        qDebug() << "Renamed RESTORE to CURRENT";
    }
    else if (chosenOs == 2)
    {

        qDebug() << "Attempting to restore Linux client.";

        //Rename previous client.
        qd->rename(chosenPath + windowsOldFile, chosenPath + restoreFile);
        qDebug() << "Renamed OLD to RESTORE.";

        //Rename unwanted client.
        qd->rename(chosenPath + windowsClient, chosenPath + windowsOldFile);
        qDebug() << "Renamed CURRENT to OLD.";

        //Rename previous client again.
        qd->rename(chosenPath + restoreFile, chosenPath + windowsClient);
        qDebug() << "Renamed RESTORE to CURRENT";
    }
    MainWindow::refreshValues();
}

void MainWindow::refreshValues()
{

    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());
    ui->updateButton->setEnabled(true);
    ui->pathText->setEnabled(true);

    if (chosenOs == 1)
    {

        MainWindow::consoleOut("Refreshing values for Linux.");
        ui->dVersionLabel->setText(cUpdater->getDLClientLinuxVersion(chosenPath + linuxClient));
        MainWindow::consoleOut("No problem with version text.");

        if (cUpdater->clientLinuxExists(chosenPath + linuxClient))
        {
            MainWindow::consoleOut("This if works.");

            ui->updateButton->setText("Update");
        }
        else
        {

            ui->updateButton->setText("Download");
        }

        if (cUpdater->oldClientLinuxExists(chosenPath + linuxOldFile))
        {

            ui->restoreButton->setEnabled(true);
        }
        else
        {

            ui->restoreButton->setEnabled(false);
        }
    }

    else if (chosenOs == 2)
    {

        MainWindow::consoleOut("Refreshing values for Windows.");
        ui->dVersionLabel->setText(cUpdater->getDLClientWindowsVersion(chosenPath + windowsClient));

        if (cUpdater->clientWindowsExists(chosenPath + windowsClient))
        {

            ui->updateButton->setText("Update");
        }
        else
        {

            ui->updateButton->setText("Download");
        }

        if (cUpdater->oldClientWindowsExists(chosenPath + windowsOldFile))
        {

            ui->restoreButton->setEnabled(true);
        }
        else
        {

            ui->restoreButton->setEnabled(false);
        }
    }

}

void MainWindow::consoleOut(QString s)
{

    qDebug() << "["+ QTime::currentTime().toString() + "] " + s;
    ui->consoleOutput->appendPlainText("[" + QTime::currentTime().toString() + "] " + s + "\n");
}

void MainWindow::on_updateButton_clicked()
{

    MainWindow::consoleOut("Download/Update button clicked.");
    ui->pathText->setEnabled(false);

    if (chosenOs == 1)
    {

        if (cUpdater->clientLinuxExists(chosenPath + linuxClient))
        {

            if (cUpdater->isCurrentLinuxClient(chosenPath + linuxClient))
            {

                MainWindow::consoleOut("Linux client does not need updated.");
                MainWindow::refreshValues();
                return;
            }
            else
            {

                MainWindow::updateClient();
                MainWindow::downloadClient();
                ui->updateButton->setText("Update");
                MainWindow::refreshValues();
                MainWindow::consoleOut("Linux client updated.");
            }
        }
        else
        {

            MainWindow::downloadClient();
            ui->updateButton->setText("Update");
            MainWindow::refreshValues();
        }
    }
    else if (chosenOs == 2)
    {

        if (cUpdater->clientWindowsExists(chosenPath + windowsClient))
        {

            if (cUpdater->isCurrentWindowsClient(chosenPath + windowsClient))
            {

                MainWindow::consoleOut("Windows client does not need updated.");
                MainWindow::refreshValues();
                return;
            }
            else
            {

                MainWindow::updateClient();
                MainWindow::downloadClient();
                ui->updateButton->setText("Update");
                MainWindow::refreshValues();
                MainWindow::consoleOut("Windows client updated.");
            }
        }
        else
        {

            MainWindow::downloadClient();
            ui->updateButton->setText("Update");
            MainWindow::refreshValues();
        }
    }
}

void MainWindow::on_restoreButton_clicked()
{

    MainWindow::consoleOut("Restore button clicked.");
    ui->pathText->setEnabled(false);

    if (chosenOs == 1)
    {

        if (cUpdater->oldClientLinuxExists(chosenPath + linuxOldFile))
        {

            MainWindow::restoreClient();
            MainWindow::refreshValues();
            MainWindow::consoleOut("Old linux client restored.");
            MainWindow::refreshValues();
        }
        else
        {

            MainWindow::consoleOut("No old linux client.");
            MainWindow::refreshValues();
            return;
        }
    }
    else if (chosenOs == 2)
    {

        if (cUpdater->oldClientWindowsExists(chosenPath + windowsOldFile))
        {

            MainWindow::restoreClient();
            MainWindow::refreshValues();
            MainWindow::consoleOut("Old windows client restored.");
            MainWindow::refreshValues();
        }
        else
        {

            MainWindow::consoleOut("No old windows client.");
            MainWindow::refreshValues();
            return;
        }
    }

}

void MainWindow::on_refreshButton_clicked()
{

    MainWindow::consoleOut("Refresh button clicked.");


    MainWindow::refreshValues();
}

void MainWindow::on_toggleConsole_clicked()
{

    MainWindow::consoleOut("Clicked console checkbox.");

    if (ui->toggleConsole->isChecked())
    {

        MainWindow::consoleOut("Showing console.");
        this->setFixedHeight(250);
        ui->consoleOutput->show();
    }
    else
    {

        MainWindow::consoleOut("Hiding console.");
        ui->consoleOutput->hide();
        this->setFixedHeight(111);
    }
}

void MainWindow::on_osSelect_activated(const QString &arg1)
{

    if (arg1 == "Linux")
    {

        MainWindow::consoleOut("Chose Linux.");
        chosenOs = 1;
        MainWindow::refreshValues();
    }
    else if (arg1 == "Windows")
    {

        MainWindow::consoleOut("Chose Windows.");
        chosenOs = 2;
        MainWindow::refreshValues();
    }
    else
    {

        MainWindow::consoleOut("Erros choosing OS. Try again.");
    }
}

void MainWindow::on_browseButton_clicked()
{

    ui->pathText->setText(QFileDialog::getExistingDirectory(this, tr("Choose client directory."), QDir::currentPath()));
}

void MainWindow::on_pathText_textChanged(const QString &arg1)
{
    if ((arg1.isEmpty()) || (arg1.isNull()) )
    {

        chosenPath = FcuDirectory;
        MainWindow::refreshValues();
        return;
    }

    chosenPath = arg1;
    MainWindow::refreshValues();
}
