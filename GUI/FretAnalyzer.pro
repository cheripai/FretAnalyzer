#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T14:31:59
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FretAnalyzer
TEMPLATE = app


SOURCES += main.cpp \
           mainwindow.cpp \
           calculate.cpp \
           dialog.cpp \
    select.cpp

HEADERS  += mainwindow.h \
            calculate.h \
            dialog.h \
    select.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc

install_it.path = $$OUT_PWD
install_it.files = fret.py
INSTALLS += install_it

win32: RC_ICONS = \
    images/logo.ico
