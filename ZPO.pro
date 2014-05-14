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
DESTDIR = ./


QMAKE_CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g
INCLUDEPATH += src

INCLUDEPATH += D:/AC601/ostatne/Programovanie/libraries/opencv/build/include
LIBS += -L"${MY_LIB_PATH}/opencv/build/x86/mingw47/lib"
LIBS += -lopencv_core248.dll
LIBS += -lopencv_highgui248.dll
LIBS += -lopencv_imgproc248.dll


HEADERS += \
    src/Mesh.h \
    src/Point.h \
    src/MeshWarpWidget.h \
    src/MainWindow.h \
    src/morph.h \
    src/Morpher.h \
    src/debug.h \
    src/Movie.h \
    src/utils.h \
    src/MoviePlayerWindow.h

SOURCES += \
    src/main.cpp \
    src/Mesh.cpp \
    src/MeshWarpWidget.cpp \
    src/MainWindow.cpp \
    src/morph.cpp \
    src/Morpher.cpp \
    src/Movie.cpp \
    src/utils.cpp \
    src/MoviePlayerWindow.cpp

FORMS += \
    src/MainWindow.ui \
    src/MoviePlayerWindow.ui
