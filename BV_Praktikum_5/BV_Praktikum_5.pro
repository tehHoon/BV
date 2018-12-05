#-------------------------------------------------
#
# Project created by QtCreator 2018-11-30T12:58:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BV_Praktikum_5
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    prak5.cpp

HEADERS += \
        mainwindow.h \
    prak5.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += C:\opencv\opencv\release\install\include
LIBS += -LC:\opencv\opencv\release\bin\
        -lopencv_calib3d343\
        -lopencv_core343\
        -lopencv_dnn343\
        -lopencv_features2d343\
        -lopencv_flann343\
        -lopencv_highgui343\
        -lopencv_imgcodecs343\
        -lopencv_imgproc343\
        -lopencv_ml343\
        -lopencv_objdetect343\
        -lopencv_photo343\
        -lopencv_shape343\
        -lopencv_stitching343\
        -lopencv_superres343\
        -lopencv_video343\
        -lopencv_videoio343\
        -lopencv_videostab343\
        -lopencv_xfeatures2d343

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
