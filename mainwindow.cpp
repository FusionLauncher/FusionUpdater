#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->consoleOutput->appendPlainText("Starting Fusion Client Updater...");
    MainWindow::setupWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWindow()
{

    qDebug() << "Setting up window.";

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

    ui->dVersionLabel->setText(cUpdater->getDLClientVersion());
    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());
}

void MainWindow::on_updateButton_clicked()
{

    qDebug() << "Download/Update button clicked.";

    if (cUpdater->clientExists())
    {

        if (cUpdater->isCurrentClient())
        {

            return;
        }
        else
        {

            cUpdater->updateClient();
            ui->updateButton->setText("Update");
            ui->dVersionLabel->setText(cUpdater->getCRClientVersion());
            ui->restoreButton->setEnabled(true);
        }
    }
    else
        {

            cUpdater->downloadClient();
            ui->updateButton->setText("Update");
        }
}

void MainWindow::on_restoreButton_clicked()
{

    qDebug() << "Restore button Clicked.";

    if (cUpdater->oldClientExists())
    {

        cUpdater->restoreClient();
        ui->dVersionLabel->setText(cUpdater->getDLClientVersion());
    }
    else
    {

        return;
    }
}

void MainWindow::on_refreshButton_clicked()
{

    qDebug() << "Refresh button clicked.";

    ui->dVersionLabel->setText(cUpdater->getDLClientVersion());
    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());
}
