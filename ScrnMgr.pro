QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configmanager/configmanager.cpp \
    ui_components/capture/capturecontainer.cpp \
    ui_components/capture/capturerenderer.cpp \
    ui_components/logtextedit.cpp \
    ui_components/presets/presetobject.cpp \
    main.cpp \
    mainwindow.cpp \
    logcat/logcat.cpp \
    snapper/snapper.cpp

HEADERS += \
    configmanager/configmanager.h \
    ui_components/capture/capturecontainer.h \
    ui_components/capture/capturerenderer.h \
    ui_components/logtextedit.h \
    ui_components/presets/presetobject.h \
    mainwindow.h \
    logcat/logcat.h \
    snapper/snapper.h

FORMS += \
    mainwindow.ui


RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
