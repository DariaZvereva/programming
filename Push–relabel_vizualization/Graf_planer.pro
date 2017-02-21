#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T20:05:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graf_planer
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    push_relable.cpp \
    netvision.cpp \
    network.cpp

HEADERS  += mainwindow.h \
    push_relable.h \
    netvision.h \
    network.h

FORMS    += mainwindow.ui
