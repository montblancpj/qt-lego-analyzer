CONFIG += c++11

INCLUDEPATH += $$PWD

TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

include(./images/images.pri)
include(./osc/osc.pri)

OTHER_FILES +=

HEADERS +=
