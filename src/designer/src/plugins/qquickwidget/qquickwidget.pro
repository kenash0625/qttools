TARGET      = QskPushButton2
QT         += widgets quickwidgets

PLUGIN_CLASS_NAME = QskPushButton2Plugin
include(../plugins.pri)

SOURCES += qquickwidget_plugin.cpp
HEADERS += qquickwidget_plugin.h
RESOURCES += qquickwidget_plugin.qrc

INCLUDEPATH+=D:/prog/qskinny-master/build/install/include/qskinny
LIBS+=D:/prog/qskinny-master/build/install/lib/qskinny/qskinny.lib
