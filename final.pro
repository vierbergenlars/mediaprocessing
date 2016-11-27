QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    graphicstile.cpp \
    worldcontroller.cpp \
    mainwindow.cpp \
    pathfinder.cpp

unix:!macx: LIBS += -L$$PWD/libworld.so/ -lworld

INCLUDEPATH += $$PWD/libworld.so
DEPENDPATH += $$PWD/libworld.so

DISTFILES += \
    libworld.so/libworld.so \
    libworld.so/libworld.so.1 \
    libworld.so/libworld.so.1.0 \
    libworld.so/libworld.so.1.0.0

HEADERS += \
    libworld.so/world.h \
    libworld.so/world_global.h \
    graphicstile.h \
    matrix.h \
    worldcontroller.h \
    mainwindow.h \
    pathfinder.h
