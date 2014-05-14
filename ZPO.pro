#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T20:27:41
#
#-------------------------------------------------

# Setup helper variables
SRC_ROOT = ./src
RES_ROOT = ./res

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

# Resources that shall be compiled into the binary
RESOURCES += \
  $${RES_ROOT}/res.qrc

# Other files (resource icons, stylesheets)
OTHER_FILES += \
  $${RES_ROOT}/style.qss \
  $${RES_ROOT}/style_windows.qss \
  $${RES_ROOT}/qss_icons/rc/Hmovetoolbar.png \
  $${RES_ROOT}/qss_icons/rc/checkbox.png \
  $${RES_ROOT}/qss_icons/rc/sizegrip.png \
  $${RES_ROOT}/qss_icons/rc/Hsepartoolbar.png \
  $${RES_ROOT}/qss_icons/rc/close.png \
  $${RES_ROOT}/qss_icons/rc/stylesheet-branch-end.png \
  $${RES_ROOT}/qss_icons/rc/Vmovetoolbar.png \
  $${RES_ROOT}/qss_icons/rc/down_arrow.png \
  $${RES_ROOT}/qss_icons/rc/stylesheet-branch-more.png \
  $${RES_ROOT}/qss_icons/rc/Vsepartoolbar.png \
  $${RES_ROOT}/qss_icons/rc/down_arrow_disabled.png \
  $${RES_ROOT}/qss_icons/rc/stylesheet-vline.png \
  $${RES_ROOT}/qss_icons/rc/branch_closed-on.png \
  $${RES_ROOT}/qss_icons/rc/left_arrow.png \
  $${RES_ROOT}/qss_icons/rc/transparent.png \
  $${RES_ROOT}/qss_icons/rc/branch_closed.png \
  $${RES_ROOT}/qss_icons/rc/left_arrow_disabled.png \
  $${RES_ROOT}/qss_icons/rc/undock.png \
  $${RES_ROOT}/qss_icons/rc/branch_open-on.png \
  $${RES_ROOT}/qss_icons/rc/right_arrow.png \
  $${RES_ROOT}/qss_icons/rc/up_arrow.png \
  $${RES_ROOT}/qss_icons/rc/branch_open.png \
  $${RES_ROOT}/qss_icons/rc/right_arrow_disabled.png \
  $${RES_ROOT}/qss_icons/rc/up_arrow_disabled.png
