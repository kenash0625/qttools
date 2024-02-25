QT += core-private widgets xml network designer-private designercomponents-private
qtHaveModule(printsupport): QT += printsupport

INCLUDEPATH += \
    ../lib/sdk \
    ../lib/extension \
    ../lib/shared \
    extra \
    D:/prog/qt-creator-opensource-src-4.13.0 \
    D:\prog\qt-creator-opensource-src-4.13.0\src\libs\
    D:\prog\qt-creator-opensource-src-4.13.0\src\plugins \
    D:\prog\qt-creator-opensource-src-4.13.0\src\libs\3rdparty

unix:LIBS += -L/usr/local/lib -lmath
win32:LIBS += D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/Core.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/QtSupport.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/ExtensionSystem.lib\
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/Utils.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/TextEditor.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/CppTools.lib \
        D:/prog/qt-creator-opensource-src-4.13.0/build/lib/qtcreator/plugins/CppEditor.lib

RESOURCES += designer.qrc \
    $$PWD/../core_qss/qss.qrc\
    $$PWD/../core_qss/images.qrc

contains(QT_CONFIG, static) {
    DEFINES += QT_DESIGNER_STATIC
}

include(../../../shared/fontpanel/fontpanel.pri)
include(../../../shared/qttoolbardialog/qttoolbardialog.pri)
include(../Qt-Frameless-Window-DarkStyle-master/frameless_window_dark.pri)

QMAKE_DOCS = $$PWD/doc/qtdesigner.qdocconf

HEADERS += \
    hometemplateselection.h \
    qdesigner.h \
    qdesigner_toolwindow.h \
    qdesigner_formwindow.h \
    qdesigner_workbench.h \
    qdesigner_settings.h \
    qdesigner_actions.h \
    qdesigner_server.h \
    qdesigner_appearanceoptions.h \
    saveformastemplate.h \
    newform.h \
    versiondialog.h \
    designer_enums.h \
    appfontdialog.h \
    preferencesdialog.h \
    assistantclient.h \
    mainwindow.h

SOURCES += main.cpp \
    hometemplateselection.cpp \
    qdesigner.cpp \
    qdesigner_toolwindow.cpp \
    qdesigner_formwindow.cpp \
    qdesigner_workbench.cpp \
    qdesigner_settings.cpp \
    qdesigner_server.cpp \
    qdesigner_actions.cpp \
    qdesigner_appearanceoptions.cpp \
    saveformastemplate.cpp \
    newform.cpp \
    versiondialog.cpp \
    appfontdialog.cpp \
    preferencesdialog.cpp \
    assistantclient.cpp \
    mainwindow.cpp

PRECOMPILED_HEADER=qdesigner_pch.h

FORMS += saveformastemplate.ui \
    hometemplateselection.ui \
    preferencesdialog.ui \
    qdesigner_appearanceoptions.ui

QMAKE_TARGET_DESCRIPTION = Qt Designer

win32 {
   RC_ICONS = designer.ico
   VERSION = $${QT_VERSION}.0
} else {
   VERSION = $${QT_VERSION}
}

mac {
    ICON = designer.icns
    QMAKE_INFO_PLIST = Info_mac.plist
    TARGET = Designer
    FILETYPES.files = uifile.icns
    FILETYPES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += FILETYPES
}

unix:!osx:!haiku:LIBS += -lm

load(qt_app)
