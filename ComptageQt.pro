#-------------------------------------------------
#
# Project created by QtCreator 2013-12-04T11:11:10
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ComptageQt
CONFIG   += console
CONFIG   -= app_bundle


TEMPLATE = app


SOURCES += main.cpp \
    bloc.cpp

HEADERS += \
    bloc.h

INCLUDEPATH +="/home/antoine/Dropbox/Antoine/opencv/opencv-2.4.7/release/include"
LIBS += -L "/home/antoine/Dropbox/Antoine/opencv/opencv-2.4.7/release/lib" -lopencv_core -lopencv_highgui
