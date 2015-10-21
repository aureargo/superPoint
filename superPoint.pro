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
    smallpt.cpp \
    convolution.cpp \
    main.cpp \
    lumiere/lumiere.cpp \
    Object/material/diffuse.cpp \
    Object/material/glass.cpp \
    Object/material/mirror.cpp \
    Object/primitive/triangle.cpp \
    Object/primitive/sphere.cpp \
    Object/primitive/box.cpp \
    Object/primitive/mesh.cpp

HEADERS += \
    scene.h \
    convolution.h \
    global.h \
    smallpt.h \
    ray.h \
    Object/ObjectTpl.h \
    Object/Object.h \
    lumiere/lumiere.h \
    lumiere/lux.h \
    Object/material/diffuse.h \
    Object/material/glass.h \
    Object/material/mirror.h \
    Object/primitive/triangle.h \
    Object/primitive/sphere.h \
    Object/primitive/box.h \
    Object/primitive/mesh.h \
    Object/primitive/primitive.h \
    Object/material/material.h
