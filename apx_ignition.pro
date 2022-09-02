QT += widgets core serialport


TARGET = $$qtLibraryTarget(apx_pluginArson_v2_)$$system(date +%Y)
TEMPLATE = lib

CONFIG += plugin c++17 silent

DESTDIR = total/
OBJECTS_DIR = obj/plugin
UI_DIR = $$OBJECTS_DIR
MOC_DIR = $$OBJECTS_DIR
RCC_DIR = $$OBJECTS_DIR


#SDK
SDK_INC = //home/tehnokrat/CLionProjects/apx_sdk
INCLUDEPATH += \
     $$SDK_INC/include/ApxGcs \
     $$SDK_INC/include/ApxCore \
     $$SDK_INC/include/ApxData \
     $$SDK_INC/include/ApxShared

LIBS += -L/$$SDK_INC/lib \
    -lApxCore \
    -lApxData \
    -lApxGcs \
    -lApxShared

target.path = total/
INSTALLS += target

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    widget.cpp

HEADERS += \
    Config_Plugin.h \
    widget.h

FORMS += \
    widget.ui

DISTFILES +=

RESOURCES += \
    images.qrc
