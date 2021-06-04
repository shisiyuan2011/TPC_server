QT       += core gui
QT       += network
QT       += widgets printsupport
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    main.cpp \
    myserver.cpp \
    myserverthread.cpp \
    mysocket.cpp \
    qcustomplot.cpp \
    widget.cpp

HEADERS += \
    database.h \
    myserver.h \
    myserverthread.h \
    mysocket.h \
    qcustomplot.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
