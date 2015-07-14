#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setupWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWindow()
{

    if (true == true)//cUpdater->clientExists())
    {

        ui->updateButton->setText("Update");
    }
    else
    {

        ui->updateButton->setText("Download");
    }
    ui->dVersionLabel->setText(cUpdater->getDLClientVersion());
    ui->cVersionLabel->setText(cUpdater->getCRClientVersion());
}

void MainWindow::on_updateButton_clicked()
{

}

void MainWindow::on_restoreButton_clicked()
{

}
