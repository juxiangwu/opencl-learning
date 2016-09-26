#-------------------------------------------------
#
# Project created by QtCreator 2016-09-26T13:54:54
#
#-------------------------------------------------

QT       -= core gui

TARGET = cl-tinyenginelib
TEMPLATE = lib

DEFINES += CLTINYENGINELIB_LIBRARY

SOURCES += clengine.cpp

HEADERS += clengine.h\
        cl-tinyenginelib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
