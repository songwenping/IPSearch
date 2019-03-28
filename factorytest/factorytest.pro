#-------------------------------------------------
#
# Project created by QtCreator 2018-11-23T08:57:44
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += concurrent
QT       += xml
QT       += script
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = factorytest
TEMPLATE = app


SOURCES += main.cpp\
        factorytest.cpp \
    mesmanager.cpp \
    qudpmanager.cpp \
    widgets/mainwidget.cpp \
    function/sensordlg.cpp \
    function/tfcarddlg.cpp \
    function/lightdlg.cpp \
    function/hdimagedlg.cpp \
    function/uniondlg.cpp \
    function/gps4gdlg.cpp \
    function/snprinterdlg.cpp \
    function/sensorfocusdlg.cpp \
    function/focuslengthdlg.cpp \
    widgets/newmessagebox.cpp \
    widgets/vlcmanager.cpp \
    widgets/snprinterwidget.cpp \
    widgets/serverset.cpp \
    widgets/keypushbutton.cpp \
    widgets/bannerwidget.cpp \
    printermanager.cpp \
    qrencode/bitstream.c \
    qrencode/mask.c \
    qrencode/mmask.c \
    qrencode/mqrspec.c \
    qrencode/qrencode.c \
    qrencode/qrinput.c \
    qrencode/qrspec.c \
    qrencode/rscode.c \
    qrencode/split.c

HEADERS  += factorytest.h \
    mesmanager.h \
    qudpmanager.h \
    widgets/mainwidget.h \
    function/sensordlg.h \
    function/tfcarddlg.h \
    function/lightdlg.h \
    function/hdimagedlg.h \
    function/uniondlg.h \
    function/gps4gdlg.h \
    function/snprinterdlg.h \
    function/sensorfocusdlg.h \
    function/focuslengthdlg.h \
    widgets/newmessagebox.h \
    widgets/vlcmanager.h \
    widgets/snprinterwidget.h \
    widgets/serverset.h \
    widgets/keypushbutton.h \
    function/global.h \
    widgets/bannerwidget.h \
    printermanager.h \
    qrencode/bitstream.h \
    qrencode/config.h \
    qrencode/mask.h \
    qrencode/mmask.h \
    qrencode/mqrspec.h \
    qrencode/qrencode.h \
    qrencode/qrencode_inner.h \
    qrencode/qrinput.h \
    qrencode/qrspec.h \
    qrencode/rscode.h \
    qrencode/split.h

FORMS    += factorytest.ui \
    widgets/mainwidget.ui \
    widgets/vlcmanager.ui \
    widgets/snprinterwidget.ui \
    widgets/serverset.ui \
    printermanager.ui

RESOURCES += \
    picture.qrc

INCLUDEPATH += G:/sdk1/include
LIBS+=G:/sdk1/lib/libvlc.lib
LIBS+= G:/sdk1/lib/libvlccore.lib


RC_ICONS = image/appico.ico
DEFINED += MES
