#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T22:21:18
#
#-------------------------------------------------

QT       += core gui network sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = FusionUpdater
TEMPLATE = app

DESTDIR = ../OUT
MOC_DIR = BUILD
OBJECTS_DIR = BUILD

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui



LIBS += -lLibFusion
LIBS += -L$$PWD/../OUT -lLibFusion

INCLUDEPATH += $$PWD/../LibFusion/LibFusion
DEPENDPATH += $$PWD/../LibFusion/LibFusion
