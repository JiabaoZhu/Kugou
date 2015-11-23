#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T15:47:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kugou
TEMPLATE = app
TRANSLATIONS += InternationalLanguage.ts
QTPLUGIN += qjpeg


SOURCES += main.cpp\
        Kugou.cpp \
    QSliderClicked.cpp

HEADERS  += Kugou.h \
    QSliderClicked.h

RESOURCES += \
    Kugou.qrc

FORMS    += Kugou.ui
