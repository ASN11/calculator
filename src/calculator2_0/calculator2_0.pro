QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../calc_controller.cc \
    ../calc_model.cc \
    ../credit_calc.cc \
    ../deposit_calc.cc \
    credit.cpp \
    deposit.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    ../calc_controller.h \
    ../calc_model.h \
    ../credit_calc.h \
    ../deposit_calc.h \
    credit.h \
    deposit.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    credit.ui \
    deposit.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
