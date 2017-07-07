#-------------------------------------------------
#
# Project created by QtCreator 2015-07-15T10:41:45
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = EasySerial
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    framewidget.cpp \
    serverwidget.cpp \
    handleclientsocketthread.cpp \
    deviceinfo.cpp

HEADERS  += mainwindow.h \
    framewidget.h \
    serverwidget.h \
    handleclientsocketthread.h \
    deviceinfo.h

FORMS    += mainwindow.ui \
    framewidget.ui

RESOURCES += \
    image.qrc

RC_FILE = ico.rc

DISTFILES += \
    ico.rc

LD_LIBRARY_PATH += .
