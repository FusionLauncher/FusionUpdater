#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T22:21:18
#
#-------------------------------------------------

QT       += core gui
QT       +=  network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = VersionChecker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


unix|win32: LIBS += -lLibFusion

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LibFusion/release/ -lLibFusion
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LibFusion/debug/ -lLibFusion
else:unix: LIBS += -L$$PWD/../LibFusion/ -lLibFusion

INCLUDEPATH += $$PWD/../LibFusion
DEPENDPATH += $$PWD/../LibFusion
