#-------------------------------------------------
#
# Project created by QtCreator 2021-03-01T20:46:42
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = MassifVisualizer
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

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gui-assets/snapshotlistbutton.cpp \
    gui-assets/generaltabwidget.cpp \
    parse-massif-file/parser-massif.cpp \
    parse-massif-file/snapshotitem.cpp \
    parse-massif-file/heaptreeitem.cpp \
    helper-functions.cpp \
    gui-assets/listbutton.cpp \
    gui-assets/treewidget.cpp \
    gui-assets/exethread.cpp \
    gui-assets/chart.cpp \
    gui-assets/configdialog.cpp \
    gui-assets/massifoptionsdialog.cpp

HEADERS += \
    mainwindow.h \
    gui-assets/snapshotlistbutton.h \
    gui-assets/generaltabwidget.h \
    parse-massif-file/parser-massif.h \
    parse-massif-file/snapshotitem.h \
    parse-massif-file/heaptreeitem.h \
    helper-functions.h \
    gui-assets/listbutton.h \
    gui-assets/treewidget.h \
    gui-assets/exethread.h \
    gui-assets/chart.h \
    gui-assets/configdialog.h \
    gui-assets/massifoptionsdialog.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    assets/recentFiles.txt \
    assets/bugReport.gif

