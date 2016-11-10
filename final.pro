QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

unix:!macx: LIBS += -L$$PWD/libworld.so/ -lworld

INCLUDEPATH += $$PWD/libworld.so
DEPENDPATH += $$PWD/libworld.so
