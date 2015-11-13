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
    Object/material/diffuse.cpp \
    Object/material/glass.cpp \
    Object/material/mirror.cpp \
    Object/primitive/sphere.cpp \
    lib/box.cpp \
    Object/primitive/mesh.cpp \
    lumiere/lux.cpp \
    Object/primitive/trianglePrim.cpp \
    lib/triangle.cpp \
    lib/bvh/bvh_nb_noeud.cpp \
    lib/bvh/bvh_feuille.cpp \
    lib/bvh/bvh_noeud.cpp

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
    Object/primitive/sphere.h \
    lib/box.h \
    Object/primitive/mesh.h \
    Object/primitive/primitive.h \
    Object/material/material.h \
    Object/primitive/trianglePrim.h \
    lib/bvh/triangleId.h \
    lib/triangle.h \
    lib/bvh/bvh.h \
    lib/bvh/bvh_nb_noeud.h \
    lib/bvh/bvh_feuille.h \
    lib/bvh/bvh_noeud.h
