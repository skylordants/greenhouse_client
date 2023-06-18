TEMPLATE = app
TARGET = greenhouse_client

QT = core gui
QT += widgets
QT += network
QT += mqtt

SOURCES += \
    main.cpp \
    window.cpp

HEADERS += \
	window.h
