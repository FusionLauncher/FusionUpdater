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
    MainWindow::checkFiles();
    MainWindow::refreshValues();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Make sure version file and client exists.
void MainWindow::checkFiles()
{

    //Linux Client exists & Version missing
    if ((cUpdater->fileExists(chosenPath + linuxClient)) && (!cUpdater->fileExists(chosenPath + lVersionFile)))
    {

        QMessageBox msg;
        msg.setText("Missing version file for Linux client. Please run the client again to create a new one.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
        MainWindow::consoleOut("Missing version file for Linux client.");
    }

    //Linux Client missing & Version exists
    if ((!cUpdater->fileExists(chosenPath + linuxClient)) && (cUpdater->fileExists(chosenPath + lVersionFile)))
    {

        QFile file(chosenPath + lVersionFile);
        file.remove();
        MainWindow::consoleOut("Missing Linux client but version file exists. Removed version file.");
    }

    //Windows Client exists & Version missing
    if ((cUpdater->fileExists(chosenPath + windowsClient)) && (!cUpdater->fileExists(chosenPath + wVersionFile)))
    {

        QMessageBox msg;
        msg.setText("Missing version file for Windows client. Please run the client again to create a new one.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
        MainWindow::consoleOut("Missing version file for Windows client.");
    }

    //Windows Client missing & Version exists
    if ((!cUpdater->fileExists(chosenPath + windowsClient)) && (cUpdater->fileExists(chosenPath + wVersionFile)))
    {

        QFile file(chosenPath + wVersionFile);
        file.remove();
        MainWindow::consoleOut("Missing Windows client but version file exists. Removed version file.");
    }
}

//Downloads client.
void MainWindow::downloadClient()
{

    MainWindow::consoleOut("Attempting to download client...");

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
        MainWindow::consoleOut("[ERROR] Client download reply error.");
        qDebug() << reply->errorString();
        ui->osSelect->setEnabled(true);
        MainWindow::refreshValues();
        ui->progressBar->setValue(0);
        return;
    }
    else if((chosenOs == 1) && (reply->url() != linuxClientUrl))
    {

        MainWindow::consoleOut("[ERROR] Client reply URL does not match real client URL.");
        ui->osSelect->setEnabled(true);
        MainWindow::refreshValues();
        ui->progressBar->setValue(0);
        return;
    }
    else if((chosenOs == 2) && (reply->url() != windowsClientUrl))
    {

        MainWindow::consoleOut("[ERROR] Client reply URL does not match real client URL.");
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
    if (chosenOs == 1) { cUpdater->writeVersion(cUpdater->getCRClientVersion(), chosenPath + lVersionFile); }
    else if (chosenOs == 2) { cUpdater->writeVersion(cUpdater->getCRClientVersion(), chosenPath + wVersionFile); }
    ui->osSelect->setEnabled(true);
    MainWindow::refreshValues();
    }
}

//Updates progress bar.
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

    MainWindow::consoleOut("Attempting to update client.");
    ui->pathText->setEnabled(false);

    //Rename downloaded client.
    if (chosenOs == 1)
    {

        qd->rename(chosenPath + linuxClient, chosenPath + linuxOldFile);
        qd->rename(chosenPath + lVersionFile, chosenPath + lVersionOldFile);
        MainWindow::consoleOut("Updated linux client.");
    }
    else if (chosenOs == 2)
    {

        qd->rename(chosenPath + windowsClient, chosenPath +  windowsOldFile);
        qd->rename(chosenPath + wVersionFile, chosenPath + wVersionOldFile);
        MainWindow::consoleOut("Updated windows client.");
    }
    MainWindow::refreshValues();
}

//Restores previous client.
void MainWindow::restoreClient()
{

    ui->pathText->setEnabled(false);

    if (chosenOs == 1)
    {

        MainWindow::consoleOut("Attempting to restore Linux client...");

        //Rename previous client.
        qd->rename(chosenPath + linuxOldFile, chosenPath + restoreFile);
        qd->rename(chosenPath + lVersionOldFile, chosenPath + versionRestoreFile);

        //Rename unwanted client.
        qd->rename(chosenPath + linuxClient, chosenPath + linuxOldFile);
        qd->rename(chosenPath + lVersionFile, chosenPath + lVersionOldFile);

        //Rename previous client again.
        qd->rename(chosenPath + restoreFile, chosenPath + linuxClient);
        qd->rename(chosenPath + versionRestoreFile, chosenPath + lVersionFile);
    }
    else if (chosenOs == 2)
    {

        MainWindow::consoleOut("Attempting to restore Windows client...");

        //Rename previous client.
        qd->rename(chosenPath + windowsOldFile, chosenPath + restoreFile);
        qd->rename(chosenPath + wVersionOldFile, chosenPath + versionRestoreFile);


        //Rename unwanted client.
        qd->rename(chosenPath + windowsClient, chosenPath + windowsOldFile);
        qd->rename(chosenPath + wVersionFile, chosenPath + wVersionOldFile);

        //Rename previous client again.
        qd->rename(chosenPath + restoreFile, chosenPath + windowsClient);
        qd->rename(chosenPath + versionRestoreFile, chosenPath + wVersionFile);
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

        MainWindow::consoleOut("Refreshing values for Linux...");
        ui->dVersionLabel->setText(cUpdater->getDLClientVersion(chosenPath + lVersionFile));

        if (cUpdater->fileExists(chosenPath + linuxClient))
        {

            ui->updateButton->setText("Update");
        }
        else
        {

            ui->updateButton->setText("Download");
        }

        if (cUpdater->fileExists(chosenPath + linuxOldFile))
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

        MainWindow::consoleOut("Refreshing values for Windows...");
        ui->dVersionLabel->setText(cUpdater->getDLClientVersion(chosenPath + wVersionFile));

        if (cUpdater->fileExists(chosenPath + windowsClient))
        {

            ui->updateButton->setText("Update");
        }
        else
        {

            ui->updateButton->setText("Download");
        }

        if (cUpdater->fileExists(chosenPath + windowsOldFile))
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

    ui->pathText->setEnabled(false);

    if (chosenOs == 1)
    {

        if (cUpdater->fileExists(chosenPath + linuxClient))
        {

            if (cUpdater->isCurrentClient(chosenPath + lVersionFile))
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

        if (cUpdater->fileExists(chosenPath + windowsClient))
        {

            if (cUpdater->isCurrentClient(chosenPath + wVersionFile))
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

    ui->pathText->setEnabled(false);

    if (chosenOs == 1)
    {

        if (cUpdater->fileExists(chosenPath + linuxOldFile))
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

        if (cUpdater->fileExists(chosenPath + windowsOldFile))
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

    MainWindow::refreshValues();
}

void MainWindow::on_toggleConsole_clicked()
{

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

        ui->pathText->setText(FcuDirectory);
        chosenPath = FcuDirectory;
        MainWindow::refreshValues();
        return;
    }

    chosenPath = arg1;
    MainWindow::consoleOut("Set directory to " + chosenPath);
    MainWindow::refreshValues();
}
