#-------------------------------------------------
#
# Project created by QtCreator 2015-01-10T15:24:40
#
#-------------------------------------------------

QT       += core widgets network concurrent
CONFIG += c++11

TARGET = QtHue
TEMPLATE = app


SOURCES += main.cpp \
    qhuelight.cpp \
    qhuebridge.cpp \
    qhuebridgemanager.cpp \
    tcpsender.cpp

HEADERS  += \
    qhuelight.h \
    qhuebridge.h \
    qhuebridgemanager.h \
    qhueerror.h \
    tcpsender.h


FORMS += \
    tcpsender.ui
