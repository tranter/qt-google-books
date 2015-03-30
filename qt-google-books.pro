#-------------------------------------------------
#
# Project created by QtCreator 2012-01-04T09:56:12
#
#-------------------------------------------------

QT       += core gui webkit network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = qtbooks
TEMPLATE = app

INCLUDEPATH += ../qjson/include ../qt-oauth-lib

linux* {
LIBS += ../qjson/build/lib/libqjson.so -L../qt-oauth-lib -l qt-oauth-lib
}

win* {
    LIBS += ../qjson/build/lib/qjson0.dll
    debug {
        LIBS += -L../qt-oauth-lib/debug -l qt-oauth-lib
    }
    release {
        LIBS += -L../qt-oauth-lib/release -l qt-oauth-lib
    }
}

macx* {
LIBS += -F../qjson/build/lib -framework qjson -L../qt-oauth-lib -l qt-oauth-lib
}

SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    books_data_manager.cpp \
    finddialog.cpp \
    reading_dialog.cpp

HEADERS  += mainwindow.h \
    form.h \
    books_data_manager.h \
    finddialog.h \
    reading_dialog.h

FORMS    += mainwindow.ui \
    form.ui \
    finddialog.ui \
    reading_dialog.ui

RESOURCES += \
    resource.qrc



















