TEMPLATE = lib
CONFIG += staticlib c++17 create_prl
CONFIG -= app_bundle
QT = core

TARGET = common

SOURCES += common.cpp
HEADERS += protocol.h settings.h