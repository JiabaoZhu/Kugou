#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T15:47:48
#
#-------------------------------------------------

QT       += core gui
QT       += webkit
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kugou
TEMPLATE = app
TRANSLATIONS += InternationalLanguage.ts
QTPLUGIN += qjpeg


#预编译
CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

#启用并行编译
QMAKE_CXXFLAGS += /MP

#生成VC工程文件
#命令 qmake -tp vc

SOURCES += main.cpp\
        Kugou.cpp \
    QSliderClicked.cpp \
    KugouUrl.cpp

HEADERS  += Kugou.h \
    QSliderClicked.h \
    KugouUrl.h \
    stable.h

RESOURCES += \
    Kugou.qrc

FORMS    += Kugou.ui
