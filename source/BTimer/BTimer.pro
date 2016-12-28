#-------------------------------------------------
#
# Project created by QtCreator 2016-12-28T10:01:21
#
#-------------------------------------------------

QT       -= core gui

TARGET = BTimer
TEMPLATE = lib

DEFINES += BTIMER_LIBRARY

INCLUDEPATH += ./../
SOURCES += \
    timer.cpp

HEADERS += \
    timer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


CONFIG(debug, debug|release){
    DESTDIR  = $$PWD/../../run/debug
     TARGET =BTImer_d
 }else{
DESTDIR  = $$PWD/../../run/release
DEFINES +=BTImer_d
}

LIBDIR =$$PWD/../../run/lib
LIBS +=-L$$LIBDIR -lboost_thread
LIBS +=-L$$LIBDIR -lboost_system

