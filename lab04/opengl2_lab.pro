QT += core gui opengl

TARGET = opengl2_lab
TEMPLATE = app

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += src/settings.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/glwidget.cpp \
    src/databinding.cpp \
    src/Emitter.cpp \
    src/CS123Vector.inl \
    src/CS123Matrix.inl \
    src/CS123Matrix.cpp

HEADERS += src/vector.h \
    src/settings.h \
    src/mainwindow.h \
    src/glwidget.h \
    src/databinding.h \
    src/camera.h \
    src/CS123Algebra.h \
    src/Emitter.h \
    src/CS123Vector.h \
    src/CS123Matrix.h \
    src/CS123Common.h

FORMS += src/mainwindow.ui

OTHER_FILES +=



