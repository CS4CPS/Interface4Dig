#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T11:35:28
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DigCom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diginterface.cpp \
    modelconfig.cpp \
    drv/drv_base.cpp \
    drv/drv_console.cpp \
    drv/drv_LoopBuf.cpp \
    drv/drv_rtdb_base.cpp \
    drv/drv_rtdb_mem.cpp \
    drv/drv_sstp.cpp \
    drv/drv_tcp_server.cpp \
    drv/drv_xml.cpp

HEADERS  += mainwindow.h \
    api/Api.hpp \
    api/ApiValue.hpp \
    diginterface.h \
    sstp_def.h \
    modelconfig.h \
    drv/drv_base.h \
    drv/drv_console.h \
    drv/drv_LoopBuf.h \
    drv/drv_rtdb_base.h \
    drv/drv_rtdb_mem.h \
    drv/drv_sstp.h \
    drv/drv_tcp_server.h \
    drv/drv_xml.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/api/ -ldigapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/api/ -ldigapi
else:unix: LIBS += -L$$PWD/api/ -ldigapi

INCLUDEPATH += $$PWD/api
INCLUDEPATH += $$PWD/../drv
DEPENDPATH += $$PWD/api

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/api/ -ldigapivalue
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/api/ -ldigapivalue
else:unix: LIBS += -L$$PWD/api/ -ldigapivalue

INCLUDEPATH += $$PWD/api
DEPENDPATH += $$PWD/api
