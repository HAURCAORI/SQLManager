#-------------------------------------------------
#
# Project created by QtCreator 2022-08-27T11:26:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SQLManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sqlhandler.cpp

HEADERS += \
        mainwindow.h \
    sqlhandler.h \
    tableformat.h \
    datatable.h

FORMS += \
        mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/library/mariadb/release/ -Wl,-R$$PWD/library/mariadb/release/ -lmariadbcpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/library/mariadb/debug/ -Wl,-R$$PWD/library/mariadb/debug/ -lmariadbcpp
else:unix:!macx: LIBS += -L$$PWD/library/mariadb/ -Wl,-R$$PWD/library/mariadb/ -lmariadbcpp

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
