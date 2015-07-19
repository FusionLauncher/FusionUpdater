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
    chosenOs = 1;
    MainWindow::refreshValues();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::downloadLinuxClient()
{

    manager = new QNetworkAccessManager;
    QNetworkRequest request(clientLinuxUrl);

    reply = manager->get(request);
    ui->updateButton->setEnabled(false);
    ui->osSelect->setEnabled(false);
    ui->restoreButton->setEnabled(false);

    QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgressBar(qint64,qint64)));

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(linuxFinished(QNetworkReply*)));
}

void MainWindow::downloadWindowsClient()
{

    manager = new QNetworkAccessManager;
    QNetworkRequest request(clientWindowsUrl);

    reply = manager->get(request);
    ui->updateButton->setEnabled(false);
    ui->osSelect->setEnabled(false);
    ui->restoreButton->setEnabled(false);

    QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgressBar(qint64,qint64)));

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(windowsFinished(QNetworkReply*)));
}

void MainWindow::linuxFinished(QNetworkReply *reply)
{

    reply->deleteLater();
    reply->ignoreSslErrors();

    if(reply->error())
    {
        qDebug() << "[ERROR] Client download reply error.";
        qDebug() << reply->errorString();
        ui->updateButton->setEnabled(true);
        ui->osSelect->setEnabled(true);
    }
    else if(reply->url() != clientLinuxUrl)
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        ui->updateButton->setEnabled(true);
        ui->osSelect->setEnabled(true);
        return;
    }
    else
    {

    QByteArray ba = reply->readAll();
    QFile file (clientLinuxDirectory);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << ba;
    file.close();
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(0);
    MainWindow::consoleOut("Linux client finished downloading.");
    ui->osSelect->setEnabled(true);
    MainWindow::refreshValues();
    }
}

void MainWindow::windowsFinished(QNetworkReply *reply)
{

    reply->deleteLater();
    reply->ignoreSslErrors();

    if(reply->error())
    {
        qDebug() << "[ERROR] Client download reply error.";
        qDebug() << reply->errorString();
        ui->updateButton->setEnabled(true);
        ui->osSelect->setEnabled(true);
    }
    else if(reply->url() != clientWindowsUrl)
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        ui->updateButton->setEnabled(true);
        ui->osSelect->setEnabled(true);
        return;
    }
    else
    {

    QByteArray ba = reply->readAll();
    QFile file (clientWindowsDirectory);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << ba;
    file.close();
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(0);
    MainWindow::consoleOut("Windows client finished downloading.");
    ui->osSelect->setEnabled(true);
    MainWindow::refreshValues();
    }
}

void MainWindow::updateProgressBar(qint64 current, qint64 total)
{
    ui->updateButton->setEnabled(false);
    ui->restoreButton->setEnabled(false);

    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

void MainWindow::refreshValues()
{

    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());
    ui->updateButton->setEnabled(true);

    if (chosenOs == 1)
    {

        MainWindow::consoleOut("Refreshing values for Linux.");
        ui->dVersionLabel->setText(cUpdater->getDLClientLinuxVersion());

        if (cUpdater->clientLinuxExists())
        {

            ui->updateButton->setText("Update");
        }
        else
        {

            ui->updateButton->setText("Download");
        }

        if (cUpdater->oldClientLinuxExists())
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
        ui->dVersionLabel->setText(cUpdater->getDLClientWindowsVersion());

        if (cUpdater->clientWindowsExists())
        {

            ui->updateButton->setText("Update");
        }
        else
        {

            ui->updateButton->setText("Download");
        }

        if (cUpdater->oldClientWindowsExists())
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

    if (chosenOs == 1)
    {

        if (cUpdater->clientLinuxExists())
        {

            if (cUpdater->isCurrentLinuxClient())
            {

                MainWindow::consoleOut("Linux client does not need updated.");
                return;
            }
            else
            {

                cUpdater->updateLinuxClient();
                MainWindow::downloadLinuxClient();
                ui->updateButton->setText("Update");
                MainWindow::refreshValues();
                MainWindow::consoleOut("Linux client updated.");
            }
        }
        else
        {

            MainWindow::downloadLinuxClient();
            ui->updateButton->setText("Update");
            MainWindow::refreshValues();
        }
    }
    else if (chosenOs == 2)
    {

        if (cUpdater->clientWindowsExists())
        {

            if (cUpdater->isCurrentWindowsClient())
            {

                MainWindow::consoleOut("Windows client does not need updated.");
                return;
            }
            else
            {

                cUpdater->updateWindowsClient();
                MainWindow::downloadWindowsClient();
                ui->updateButton->setText("Update");
                MainWindow::refreshValues();
                MainWindow::consoleOut("Windows client updated.");
            }
        }
        else
        {

            MainWindow::downloadWindowsClient();
            ui->updateButton->setText("Update");
        }
    }
}

void MainWindow::on_restoreButton_clicked()
{

    MainWindow::consoleOut("Restore button clicked.");

    if (chosenOs == 1)
    {

        if (cUpdater->oldClientLinuxExists())
        {

            cUpdater->restoreLinuxClient();
            MainWindow::refreshValues();
            MainWindow::consoleOut("Old linux client restored.");
            MainWindow::refreshValues();
        }
        else
        {

            MainWindow::consoleOut("No old linux client.");
            return;
        }
    }
    else if (chosenOs == 2)
    {

        if (cUpdater->oldClientWindowsExists())
        {

            cUpdater->restoreWindowsClient();
            MainWindow::refreshValues();
            MainWindow::consoleOut("Old windows client restored.");
            MainWindow::refreshValues();
        }
        else
        {

            MainWindow::consoleOut("No old windows client.");
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
