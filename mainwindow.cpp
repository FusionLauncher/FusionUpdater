#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::consoleOut("Started Fusion client updater.");
    MainWindow::setupWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWindow()
{

    MainWindow::consoleOut("Setting up window.");

    if (cUpdater->clientExists())
    {

        ui->updateButton->setText("Update");
    }
    else
    {

        ui->updateButton->setText("Download");
    }

    if (cUpdater->oldClientExists())
    {

        ui->restoreButton->setEnabled(true);
    }
    else
    {

        ui->restoreButton->setEnabled(false);
    }

    ui->consoleOutput->hide();
    this->setFixedHeight(80);

    ui->dVersionLabel->setText(cUpdater->getDLClientVersion());
    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());
}

void MainWindow::consoleOut(QString s)
{

    qDebug() << "["+ QTime::currentTime().toString() + "] " + s;
    ui->consoleOutput->appendPlainText("[" + QTime::currentTime().toString() + "] " + s + "\n");
}

void MainWindow::on_updateButton_clicked()
{

    MainWindow::consoleOut("Download/Update button clicked.");

    if (cUpdater->clientExists())
    {

        if (cUpdater->isCurrentClient())
        {

            MainWindow::consoleOut("Client does not need updated.");

            return;
        }
        else
        {

            cUpdater->updateClient();
            ui->updateButton->setText("Update");
            ui->dVersionLabel->setText(cUpdater->getCRClientVersion());
            ui->restoreButton->setEnabled(true);
            MainWindow::consoleOut("Client updated.");
        }
    }
    else
        {

            cUpdater->downloadClient();
            ui->updateButton->setText("Update");
            MainWindow::consoleOut("Downloaded client.");
        }
}

void MainWindow::on_restoreButton_clicked()
{

    MainWindow::consoleOut("Restore button clicked.");

    if (cUpdater->oldClientExists())
    {

        cUpdater->restoreClient();
        ui->dVersionLabel->setText(cUpdater->getDLClientVersion());
        MainWindow::consoleOut("Old client restored.");
    }
    else
    {

        MainWindow::consoleOut("No old client.");
        return;
    }
}

void MainWindow::on_refreshButton_clicked()
{

    MainWindow::consoleOut("Refresh button clicked.");


    ui->dVersionLabel->setText(cUpdater->getDLClientVersion());
    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());

    MainWindow::consoleOut("Refreshed version info.");
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
