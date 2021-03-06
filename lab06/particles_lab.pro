#-------------------------------------------------
#
# Project created by QtCreator 2010-06-25T14:35:30
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = particles_lab
TEMPLATE = app

INCLUDEPATH += src
DEPENDPATH += src

HEADERS  += src/particlewidget.h \
    src/particleemitter.h \
    src/mainwindow.h \
    src/common.h \
    src/Emitter.h \
    src/CS123Vector.h \
    src/CS123Matrix.h \
    src/CS123Common.h \
    src/CS123Algebra.h

SOURCES += src/particlewidget.cpp \
    src/particleemitter.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/Emitter.cpp \
    src/CS123Vector.inl \
    src/CS123Matrix.inl \
    src/CS123Matrix.cpp

FORMS    += src/mainwindow.ui

RESOURCES += \
    textures/images.qrc

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -msse4 -ftree-vectorize -ffast-math -fmerge-all-constants -fno-branch-count-reg -foptimize-register-move -ftree-loop-linear -funroll-loops












