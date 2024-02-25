TARGET = QtDesigner
MODULE = designer

QT = core-private gui-private widgets-private xml uiplugin

DEFINES += \
    QDESIGNER_SDK_LIBRARY \
    QDESIGNER_EXTENSION_LIBRARY \
    QDESIGNER_UILIB_LIBRARY \
    QDESIGNER_SHARED_LIBRARY

static:DEFINES += QT_DESIGNER_STATIC

include(extension/extension.pri)
include(sdk/sdk.pri)
include(shared/shared.pri)
include(uilib/uilib.pri)
PRECOMPILED_HEADER=lib_pch.h

MODULE_PLUGIN_TYPES = designer
load(qt_module)

INCLUDEPATH+=D:/prog/qt-creator-opensource-src-4.13.0/src/libs \
        D:/prog/qt-creator-opensource-src-4.13.0/src/plugins/coreplugin \
        D:/prog/qt-creator-opensource-src-4.13.0/src/plugins

unix:LIBS += -L/usr/local/lib -lmath
win32:LIBS += D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/ExtensionSystem.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/Utils.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/Core.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/TextEditor.lib \
        D:/prog/qt/5.15.2/msvc2019_64/qtbase/bin/uic5.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/CppEditor.lib
