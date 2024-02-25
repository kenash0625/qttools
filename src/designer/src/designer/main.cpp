/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdesigner.h"
#include <QtCore/qlibraryinfo.h>
#include <QtCore/qdir.h>

#include <stdlib.h>
#include<D:/prog/qt-creator-opensource-src-4.13.0/build/src/app/app_version.h>
#include <extensionsystem/pluginmanager.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <utils/fileutils.h>
#include <cpptools/cppcodestylepreferences.h>
#include <cpptools/cpptoolssettings.h>
#include <cpptools/cpptoolsconstants.h>
#include <cppeditor/cppeditor.h>
#include <cppeditor/cpphighlighter.h>
#include <cppeditor/cppeditorconstants.h>
#include <texteditor/icodestylepreferencesfactory.h>
#include <texteditor/displaysettings.h>
#include <texteditor/snippets/snippetprovider.h>
#include <texteditor/textdocument.h>

#include "src/plugins/coreplugin/SetDefaultCreatorTheme.h"
#include<D:/prog/qt-creator-opensource-src-4.13.0/src/plugins/texteditor/snippets/snippeteditor.h>
#include<D:/prog/qt-creator-opensource-src-4.13.0/src/plugins/texteditor/texteditorsettings.h>
QT_USE_NAMESPACE

static bool isOptionSet(int argc, char *argv[], const char *option)
{
    for (int i = 1; i < argc; ++i) {
        if (qstrcmp(argv[i], option) == 0)
            return true;
    }
    return false;
}

static bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(Utils::FilePath::fromString(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        const QStringList fileNames = sourceDir.entryList
                                      (QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        for (const QString &fileName : fileNames) {
            const QString newSrcFilePath = srcFilePath + '/' + fileName;
            const QString newTgtFilePath = tgtFilePath + '/' + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}

static QSettings *createUserSettings()
{
    return new QSettings(QSettings::IniFormat, QSettings::UserScope,
                         QLatin1String(Core::Constants::IDE_SETTINGSVARIANT_STR),
                         QLatin1String(Core::Constants::IDE_CASED_ID));
}

static inline QSettings *userSettings()
{
    QSettings *settings = createUserSettings();
    const QString fromVariant = QLatin1String(Core::Constants::IDE_COPY_SETTINGS_FROM_VARIANT_STR);
    if (fromVariant.isEmpty())
        return settings;

           // Copy old settings to new ones:
    QFileInfo pathFi = QFileInfo(settings->fileName());
    if (pathFi.exists()) // already copied.
        return settings;

    QDir destDir = pathFi.absolutePath();
    if (!destDir.exists())
        destDir.mkpath(pathFi.absolutePath());

    QDir srcDir = destDir;
    srcDir.cdUp();
    if (!srcDir.cd(fromVariant))
        return settings;

    if (srcDir == destDir) // Nothing to copy and no settings yet
        return settings;

    const QStringList entries = srcDir.entryList();
    for (const QString &file : entries) {
        const QString lowerFile = file.toLower();
        if (lowerFile.startsWith(QLatin1String("profiles.xml"))
            || lowerFile.startsWith(QLatin1String("toolchains.xml"))
            || lowerFile.startsWith(QLatin1String("qtversion.xml"))
            || lowerFile.startsWith(QLatin1String("devices.xml"))
            || lowerFile.startsWith(QLatin1String("debuggers.xml"))
            || lowerFile.startsWith(QLatin1String(Core::Constants::IDE_ID) + "."))
            QFile::copy(srcDir.absoluteFilePath(file), destDir.absoluteFilePath(file));
        if (file == QLatin1String(Core::Constants::IDE_ID))
            copyRecursively(srcDir.absoluteFilePath(file), destDir.absoluteFilePath(file));
    }

           // Make sure to use the copied settings:
    delete settings;
    return createUserSettings();
}
//extern "C" __declspec(dllimport) QWidget*test111(QWidget*parent);
//#pragma comment(lib,"D:/prog/qt/5.15.2/msvc2019_64/qttools/plugins/designer/QskPushButtonPlugind.lib")
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(designer);
    qputenv("QSK_PLUGIN_PATH", "D:/prog/qskinny-master/build/install/lib/qskinny/plugins");
    // Attributes to be set before QApplication is constructed.
    if (isOptionSet(argc, argv, "--no-scaling")) {
        QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    } else {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    }

    // required for QWebEngineView
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QDesigner app(argc, argv);

    SetDefaultCreatorTheme();

    QSettings *settings = userSettings();
    QSettings *globalSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,
                                              QLatin1String(Core::Constants::IDE_SETTINGSVARIANT_STR),
                                              QLatin1String(Core::Constants::IDE_CASED_ID));
    ExtensionSystem::PluginManager pluginManager;
    ExtensionSystem::PluginManager::setGlobalSettings(globalSettings);
    ExtensionSystem::PluginManager::setSettings(settings);


    switch (app.parseCommandLineArguments()) {
    case QDesigner::ParseArgumentsSuccess:
        break;
    case QDesigner::ParseArgumentsError:
        return 1;
    case QDesigner::ParseArgumentsHelpRequested:
        return 0;
    }
    QGuiApplication::setQuitOnLastWindowClosed(false);
    Core::ActionManager actman(&app);
    Core::EditorManager edman(&app);
    TextEditor::TextEditorSettings textEditsetting;
    CppTools::CppToolsSettings cppToolsetting;

    TextEditor::SnippetProvider::registerGroup(CppEditor::Constants::CPP_SNIPPETS_GROUP_ID
                                               , QObject::tr("C++", "SnippetProvider"),
                                               &CppEditor::Internal::CppEditor::decorateEditor);

//    TextEditor::SnippetEditorWidget w;
//        auto displaySettings = w.displaySettings();
//        displaySettings.m_visualizeWhitespace = true;
//        w.setDisplaySettings(displaySettings);
//    TextEditor::SnippetProvider::decorateEditor(&w, "C++");
//    w.show();

//    QFile file("d:/test.h");
//    file.open(QIODevice::ReadOnly);
//    QString str=file.readAll();
//    TextEditor::TextEditorWidget edt;
//    edt.setPlainText(str);
//    auto doc=edt.document();
//    CppEditor::CppHighlighter highl;
//    edt.textDocument()->setSyntaxHighlighter(&highl);

//    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
//    {
//        highl.highlightBlock(it.text());
//    }
//    edt.show();
//    CppTools::CppCodeStylePreferences *originalCodeStylePreferences = CppTools::CppToolsSettings::instance()
//                                                                    ->cppCodeStyle();
//    auto m_pageCppCodeStylePreferences = new CppTools::CppCodeStylePreferences();
//    m_pageCppCodeStylePreferences->setDelegatingPool(
//            originalCodeStylePreferences->delegatingPool());
//    m_pageCppCodeStylePreferences->setCodeStyleSettings(
//            originalCodeStylePreferences->codeStyleSettings());
//    m_pageCppCodeStylePreferences->setCurrentDelegate(
//            originalCodeStylePreferences->currentDelegate());
//    // we set id so that it won't be possible to set delegate to the original prefs
//    m_pageCppCodeStylePreferences->setId(originalCodeStylePreferences->id());
//    auto m_widget = TextEditor::TextEditorSettings::codeStyleFactory(CppTools::Constants::CPP_SETTINGS_ID)
//                       ->createCodeStyleEditor(m_pageCppCodeStylePreferences);
//    m_widget->show();
    return QApplication::exec();
}
