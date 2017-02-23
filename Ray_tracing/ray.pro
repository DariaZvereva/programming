QT += core
QT -= gui

QMAKE_CXXFLAGS += `libpng-config --cflags`
QMAKE_LFLAGS +=`libpng-config --ldflags --libs`
CONFIG += c++11
CONFIG += pthread

TARGET = ray
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    point.cpp \
    figure.cpp \
    ray.cpp \
    triangle.cpp \
    sphere.cpp \
    projector.cpp \
    plane.cpp \
    parallelogram.cpp \
    lightsource.cpp \
    quadrilateral.cpp \
    material.cpp

HEADERS += \
    point.h \
    figure.h \
    ray.h \
    triangle.h \
    sphere.h \
    projector.h \
    plane.h \
    parallelogram.h \
    lightsource.h \
    quadrilateral.h \
    material.h \
    thread_pool.h \
    thread_safe_queue.h
