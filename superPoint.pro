#-------------------------------------------------
#
# Project created by QtCreator 2015-09-17T10:40:49
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = superPoint
CONFIG   += console
CONFIG   -= app_bundle

CONFIG  += c++11

INCLUDEPATH += C:/librairie/glm

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TEMPLATE = app


SOURCES += \
    smallpt.cpp

HEADERS += \
    Object.h \
    scene.h \
    convolution.h \
    material.h \
    primitive.h
