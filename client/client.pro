QT += core widgets network

CONFIG += c++17
TARGET = ClockClient
TEMPLATE = app

INCLUDEPATH += $$PWD/../common
DEPENDPATH  += $$PWD/../common

# --- линковка common ---
win32:CONFIG(release, debug|release): \
    LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): \
    LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: \
    LIBS += -L$$OUT_PWD/../common/ -lcommon

win32:CONFIG(release, debug|release): \
    PRE_TARGETDEPS += $$OUT_PWD/../common/release/$${QMAKE_PREFIX_STATICLIB}common.$${QMAKE_EXTENSION_STATICLIB}
else:win32:CONFIG(debug, debug|release): \
    PRE_TARGETDEPS += $$OUT_PWD/../common/debug/$${QMAKE_PREFIX_STATICLIB}common.$${QMAKE_EXTENSION_STATICLIB}
else:unix: \
    PRE_TARGETDEPS += $$OUT_PWD/../common/$${QMAKE_PREFIX_STATICLIB}common.$${QMAKE_EXTENSION_STATICLIB}

SOURCES += \
    main.cpp \
    clientwindow.cpp \
    clockface.cpp

HEADERS += \
    clientwindow.h \
    clockface.h

FORMS += \
    clientwindow.ui