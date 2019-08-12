TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += $$files(include/*.h, true)
HEADERS += $$files(include/*.hpp, true)

SOURCES += main.cpp

INCLUDEPATH += \
    3rdparty/json/include \
    3rdparty/preprocessor/include \
    include
