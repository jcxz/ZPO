#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T20:27:41
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZPO
TEMPLATE = app

OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj
UI_DIR = obj


QMAKE_CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g
INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/Mesh.cpp \
    src/MeshWarpWidget.cpp \
    src/MainWindow.cpp \
    src/morph.cpp

HEADERS += \
    src/Mesh.h \
    src/Point.h \
    src/MeshWarpWidget.h \
    src/MainWindow.h \
    src/morph.h

FORMS += \
    src/MainWindow.ui
