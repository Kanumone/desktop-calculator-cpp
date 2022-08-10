QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = C7_SmartCalc_v1.0-0

INCLUDEPATH += $$PWD/include

release: DESTDIR = build/release

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

SOURCES += \
    source/creditcalc.cpp \
    source/debitcalc.cpp \
    source/graphics.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/s21_calculator.c \
    source/s21_structures.c \
    source/s21_parser.c \
    source/qcustomplot.cpp

HEADERS += \
    include/creditcalc.h \
    include/debitcalc.h \
    include/graphics.h \
    include/mainwindow.h \
    include/s21_parser.h \
    include/s21_structures.h \
    include/s21_calculator.h \
    include/qcustomplot.h


FORMS += \
    forms/creditcalc.ui \
    forms/debitcalc.ui \
    forms/graphics.ui \
    forms/mainwindow.ui

TRANSLATIONS += \
    Calculator_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
