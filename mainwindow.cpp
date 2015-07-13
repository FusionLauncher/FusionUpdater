#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updater = new FUpdater();
    setupWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWindow()
{

    if (updater->clientExists() == true)
    {
        ui->updateButton->setText("Update");
    }
    else
    {

        ui->updateButton->setText("Download");
    }

    ui->dVersionLabel->setText(updater->getDLClientVersion());
    ui->cVersionLabel->setText((updater->getCRClientVersion()));
}

void MainWindow::on_updateButton_clicked()
{

    if (updater->clientExists())
    {

        if (updater->isCurrentClient())
        {

            return;
        }
        else
        {

            updater->updateClient();
            ui->updateButton->setText("Update");
            ui->dVersionLabel->setText(updater->getDLClientVersion());
        }
    }
    else
    {

        updater->downloadClient();
        ui->updateButton->setText("Update");
    }
}

void MainWindow::on_restoreButton_clicked()
{

    if (updater->oldClientExists())
    {

        updater->restoreClient();
        ui->dVersionLabel->setText(updater->getDLClientVersion());
    }
    else
    {

        return;
    }
}
