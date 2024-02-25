TARGET      = QskProgressBarPlugin
QT         += widgets quickwidgets

PLUGIN_CLASS_NAME = QskProgressBar2
include(../plugins.pri)

SOURCES += qquickwidget_plugin.cpp
HEADERS += qquickwidget_plugin.h
RESOURCES += qquickwidget_plugin.qrc

INCLUDEPATH+=D:/prog/qskinny-master/build/install/include/qskinny D:/prog/qskinny-master/qmlexport
LIBS+=D:/prog/qskinny-master/build/install/lib/qskinny/qskinny.lib D:/prog/qskinny-master/build/install/lib/qskinny/qskqmlexport.lib
