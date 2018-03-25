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
    ../drv/hal_qt/drv_console.cpp \
    ../drv/hal_qt/drv_LoopBuf.cpp \
    ../drv/hal_qt/drv_tcp_server.cpp \
    ../drv/hal/drv_base.cpp \
    ../drv/hal/drv_sstp.cpp \
    ../drv/rtdb/drv_rtdb_base.cpp \
    ../drv/rtdb/drv_rtdb_mem.cpp \
    modelconfig.cpp \
    ../drv/hal_qt/drv_xml.cpp \
    ../drv/hal_qt/drv_xml.cpp

HEADERS  += mainwindow.h \
    api/Api.hpp \
    api/ApiValue.hpp \
    diginterface.h \
    ../drv/hal_qt/drv_console.h \
    ../drv/hal_qt/drv_LoopBuf.h \
    ../drv/hal_qt/drv_tcp_server.h \
    ../drv/hal/drv_base.h \
    ../drv/hal/drv_sstp.h \
    sstp_def.h \
    ../drv/rtdb/drv_rtdb_base.h \
    ../drv/rtdb/drv_rtdb_mem.h \
    modelconfig.h \
    ../drv/hal_qt/drv_xml.h \
    ../drv/hal_qt/drv_xml.h

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
