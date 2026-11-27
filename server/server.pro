QT += core widgets network

CONFIG += c++17
TARGET = ClockServer
TEMPLATE = app

INCLUDEPATH += $$PWD/../common
DEPENDPATH  += $$PWD/../common

# Static lib built by common.pro lives in a per-configuration subdirectory
# (common/debug or common/release) when CONFIG contains debug_and_release.
CONFIG(debug, debug|release): COMMON_LIB_DIR = $$OUT_PWD/../common/debug
else: COMMON_LIB_DIR = $$OUT_PWD/../common/release

LIBS += -L$$COMMON_LIB_DIR -lcommon
PRE_TARGETDEPS += $$COMMON_LIB_DIR/$${QMAKE_PREFIX_STATICLIB}common.$${QMAKE_EXTENSION_STATICLIB}

SOURCES += \
    main.cpp \
    serverwindow.cpp

HEADERS += \
    serverwindow.h

FORMS += \
    serverwindow.ui