/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLabel *dVersionLabel;
    QLabel *label_2;
    QLabel *cVersionLabel;
    QPushButton *updateButton;
    QPushButton *restoreButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(183, 85);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 101, 16));
        dVersionLabel = new QLabel(centralWidget);
        dVersionLabel->setObjectName(QString::fromUtf8("dVersionLabel"));
        dVersionLabel->setGeometry(QRect(120, 10, 41, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 30, 81, 16));
        cVersionLabel = new QLabel(centralWidget);
        cVersionLabel->setObjectName(QString::fromUtf8("cVersionLabel"));
        cVersionLabel->setGeometry(QRect(100, 30, 47, 13));
        updateButton = new QPushButton(centralWidget);
        updateButton->setObjectName(QString::fromUtf8("updateButton"));
        updateButton->setGeometry(QRect(10, 50, 75, 23));
        restoreButton = new QPushButton(centralWidget);
        restoreButton->setObjectName(QString::fromUtf8("restoreButton"));
        restoreButton->setGeometry(QRect(100, 50, 75, 23));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Fusion Updater", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Downloaded version: ", 0, QApplication::UnicodeUTF8));
        dVersionLabel->setText(QApplication::translate("MainWindow", "dVersion", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Current version: ", 0, QApplication::UnicodeUTF8));
        cVersionLabel->setText(QApplication::translate("MainWindow", "cVersion", 0, QApplication::UnicodeUTF8));
        updateButton->setText(QApplication::translate("MainWindow", "Error", 0, QApplication::UnicodeUTF8));
        restoreButton->setText(QApplication::translate("MainWindow", "Restore", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
