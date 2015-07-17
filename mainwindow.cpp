#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::consoleOut("Started Fusion client updater.");
    ui->consoleOutput->hide();
    this->setFixedHeight(80);
    ui->osSelect->addItem("Linux");
    ui->osSelect->addItem("Windows");
    chosenOs = 1;
    MainWindow::refreshValues();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshValues()
{

    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());

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
                ui->updateButton->setText("Update");
                ui->dVersionLabel->setText(cUpdater->getCRClientVersion());
                ui->restoreButton->setEnabled(true);
                MainWindow::consoleOut("Linux client updated.");
            }
        }
        else
        {

            cUpdater->downloadLinuxClient();
            ui->updateButton->setText("Update");
            MainWindow::consoleOut("Downloaded linux client.");
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
                ui->updateButton->setText("Update");
                ui->dVersionLabel->setText(cUpdater->getCRClientVersion());
                ui->restoreButton->setEnabled(true);
                MainWindow::consoleOut("Windows client updated.");
            }
        }
        else
        {

            cUpdater->downloadWindowsClient();
            ui->updateButton->setText("Update");
            MainWindow::consoleOut("Downloaded Windows client.");
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
            ui->dVersionLabel->setText(cUpdater->getDLClientLinuxVersion());
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
            ui->dVersionLabel->setText(cUpdater->getDLClientWindowsVersion());
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
        this->setFixedHeight(219);
        ui->consoleOutput->show();
    }
    else
    {

        MainWindow::consoleOut("Hiding console.");
        ui->consoleOutput->hide();
        this->setFixedHeight(80);
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
