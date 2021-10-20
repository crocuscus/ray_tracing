TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS +=  -fopenmp

SOURCES += \
main.cpp \
scene.cpp \
base_geom.cpp \
sphere.cpp \
triangle.cpp

HEADERS += \
    triangle.h \
figure.h \
color.h \
    plane.h \
scene.h \
base_geom.h \
sphere.h

DISTFILES +=
    README
