/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
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
****************************************************************************/

#include "debuggeractions.h"
#include "debuggericons.h"

#ifdef Q_OS_WIN
#include "registerpostmortemaction.h"
#endif

#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>
#include <utils/savedaction.h>
#include <utils/qtcassert.h>

#include <QDebug>
#include <QSettings>

using namespace Utils;

static const char debugModeSettingsGroupC[] = "DebugMode";
static const char cdbSettingsGroupC[] = "CDB2";
static const char sourcePathMappingArrayNameC[] = "SourcePathMappings";
static const char sourcePathMappingSourceKeyC[] = "Source";
static const char sourcePathMappingTargetKeyC[] = "Target";

namespace Debugger {
namespace Internal {

void GlobalDebuggerOptions::toSettings() const
{
    QSettings *s = Core::ICore::settings();
    s->beginWriteArray(sourcePathMappingArrayNameC);
    if (!sourcePathMap.isEmpty() || !sourcePathRegExpMap.isEmpty()) {
        const QString sourcePathMappingSourceKey(sourcePathMappingSourceKeyC);
        const QString sourcePathMappingTargetKey(sourcePathMappingTargetKeyC);
        int i = 0;
        for (auto it = sourcePathMap.constBegin(), cend = sourcePathMap.constEnd();
             it != cend;
             ++it, ++i) {
            s->setArrayIndex(i);
            s->setValue(sourcePathMappingSourceKey, it.key());
            s->setValue(sourcePathMappingTargetKey, it.value());
        }
        for (auto it = sourcePathRegExpMap.constBegin(), cend = sourcePathRegExpMap.constEnd();
             it != cend;
             ++it, ++i) {
            s->setArrayIndex(i);
            s->setValue(sourcePathMappingSourceKey, it->first.pattern());
            s->setValue(sourcePathMappingTargetKey, it->second);
        }
    }
    s->endArray();
}

void GlobalDebuggerOptions::fromSettings()
{
    QSettings *s = Core::ICore::settings();
    sourcePathMap.clear();
    if (const int count = s->beginReadArray(sourcePathMappingArrayNameC)) {
        const QString sourcePathMappingSourceKey(sourcePathMappingSourceKeyC);
        const QString sourcePathMappingTargetKey(sourcePathMappingTargetKeyC);
        for (int i = 0; i < count; ++i) {
             s->setArrayIndex(i);
             const QString key = s->value(sourcePathMappingSourceKey).toString();
             const QString value = s->value(sourcePathMappingTargetKey).toString();
             if (key.startsWith('('))
                 sourcePathRegExpMap.append(qMakePair(QRegularExpression(key), value));
             else
                 sourcePathMap.insert(key, value);
        }
    }
    s->endArray();
}

//////////////////////////////////////////////////////////////////////////
//
// DebuggerSettings
//
//////////////////////////////////////////////////////////////////////////

static DebuggerSettings *theDebuggerSettings = nullptr;

DebuggerSettings::DebuggerSettings()
{
    theDebuggerSettings = this;

    const QString debugModeGroup(debugModeSettingsGroupC);
    const QString cdbSettingsGroup(cdbSettingsGroupC);

    SavedAction *item = nullptr;

    item = new SavedAction;
    insertItem(SettingsDialog, item);
    item->setText(tr("Configure Debugger..."));

    //
    // View
    //
    item = new SavedAction;
    item->setText(tr("Always Adjust View Column Widths to Contents"));
    item->setCheckable(true);
    item->setValue(true);
    item->setDefaultValue(true);
    item->setSettingsKey(debugModeGroup, "AlwaysAdjustColumnWidths");
    insertItem(AlwaysAdjustColumnWidths, item);

    // Needed by QML Inspector
    item = new SavedAction;
    item->setText(tr("Use Alternating Row Colors"));
    item->setSettingsKey(debugModeGroup, "UseAlternatingRowColours");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(UseAlternatingRowColors, item);

    item = new SavedAction;
    item->setText(tr("Keep Editor Stationary When Stepping"));
    item->setSettingsKey(debugModeGroup, "StationaryEditorWhileStepping");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(StationaryEditorWhileStepping, item);

    item = new SavedAction;
    item->setText(tr("Debugger Font Size Follows Main Editor"));
    item->setSettingsKey(debugModeGroup, "FontSizeFollowsEditor");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(FontSizeFollowsEditor, item);

    item = new SavedAction;
    item->setText(tr("Show a Message Box When Receiving a Signal"));
    item->setSettingsKey(debugModeGroup, "UseMessageBoxForSignals");
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(UseMessageBoxForSignals, item);

    item = new SavedAction;
    item->setText(tr("Log Time Stamps"));
    item->setSettingsKey(debugModeGroup, "LogTimeStamps");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(LogTimeStamps, item);

    item = new SavedAction;
    item->setText(tr("Dereference Pointers Automatically"));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setSettingsKey(debugModeGroup, "AutoDerefPointers");
    item->setToolTip(tr("<p>This switches the Locals and Expressions views to "
        "automatically dereference pointers. This saves a level in the "
        "tree view, but also loses data for the now-missing intermediate "
        "level."));
    insertItem(AutoDerefPointers, item);

    //
    // Cdb Options
    //

    item = new SavedAction;
    item->setDefaultValue(QString());
    item->setSettingsKey(cdbSettingsGroup, "AdditionalArguments");
    insertItem(CdbAdditionalArguments, item);

    item = new SavedAction;
    item->setDefaultValue(QStringList());
    item->setSettingsKey(cdbSettingsGroup, "SymbolPaths");
    insertItem(CdbSymbolPaths, item);

    item = new SavedAction;
    item->setDefaultValue(QStringList());
    item->setSettingsKey(cdbSettingsGroup, "SourcePaths");
    insertItem(CdbSourcePaths, item);

    item = new SavedAction;
    item->setDefaultValue(QStringList());
    item->setSettingsKey(cdbSettingsGroup, "BreakEvent");
    insertItem(CdbBreakEvents, item);

    item = new SavedAction;
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setSettingsKey(cdbSettingsGroup, "BreakOnCrtDbgReport");
    insertItem(CdbBreakOnCrtDbgReport, item);

    item = new SavedAction;
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setSettingsKey(cdbSettingsGroup, "CDB_Console");
    insertItem(UseCdbConsole, item);

    item = new SavedAction;
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setSettingsKey(cdbSettingsGroup, "BreakpointCorrection");
    insertItem(CdbBreakPointCorrection, item);

    item = new SavedAction;
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setSettingsKey(cdbSettingsGroup, "UsePythonDumper");
    insertItem(CdbUsePythonDumper, item);

    item = new SavedAction;
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setSettingsKey(cdbSettingsGroup, "FirstChanceExceptionTaskEntry");
    insertItem(FirstChanceExceptionTaskEntry, item);

    item = new SavedAction;
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setSettingsKey(cdbSettingsGroup, "SecondChanceExceptionTaskEntry");
    insertItem(SecondChanceExceptionTaskEntry, item);

    item = new SavedAction;
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setSettingsKey(cdbSettingsGroup, "IgnoreFirstChanceAccessViolation");
    insertItem(IgnoreFirstChanceAccessViolation, item);

    //
    // Locals & Watchers
    //
    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "ShowStandardNamespace");
    item->setText(tr("Show \"std::\" Namespace in Types"));
    item->setDialogText(tr("Show \"std::\" namespace in types"));
    item->setToolTip(tr("<p>Shows \"std::\" prefix for types from the standard library."));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(ShowStdNamespace, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "ShowQtNamespace");
    item->setText(tr("Show Qt's Namespace in Types"));
    item->setDialogText(tr("Show Qt's namespace in types"));
    item->setToolTip(tr("<p>Shows Qt namespace prefix for Qt types. This is only "
                        "relevant if Qt was configured with \"-qtnamespace\"."));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(ShowQtNamespace, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "ShowQObjectNames2");
    item->setText(tr("Show QObject names if available"));
    item->setDialogText(tr("Show QObject names if available"));
    item->setToolTip(tr("<p>Displays the objectName property of QObject based items. "
                        "Note that this can negatively impact debugger performance "
                        "even if no QObjects are present."));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(ShowQObjectNames, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "SortStructMembers");
    item->setText(tr("Sort Members of Classes and Structs Alphabetically"));
    item->setDialogText(tr("Sort members of classes and structs alphabetically"));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(SortStructMembers, item);

    //
    // DebuggingHelper
    //
    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "UseDebuggingHelper");
    item->setText(tr("Use Debugging Helpers"));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(UseDebuggingHelpers, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "UseCodeModel");
    item->setDialogText(tr("Use code model"));
    item->setToolTip(tr("<p>Selecting this causes the C++ Code Model being asked "
      "for variable scope information. This might result in slightly faster "
      "debugger operation but may fail for optimized code."));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(UseCodeModel, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "ShowThreadNames");
    item->setToolTip(tr("<p>Displays names of QThread based threads."));
    item->setDialogText(tr("Display thread names"));
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    insertItem(ShowThreadNames, item);


    //
    // Breakpoints
    //
    item = new SavedAction;
    item->setText(tr("Synchronize Breakpoints"));
    insertItem(SynchronizeBreakpoints, item);

    item = new SavedAction;
    item->setText(tr("Adjust Breakpoint Locations"));
    item->setToolTip(tr("<p>Not all source code lines generate "
      "executable code. Putting a breakpoint on such a line acts as "
      "if the breakpoint was set on the next line that generated code. "
      "Selecting 'Adjust Breakpoint Locations' shifts the red "
      "breakpoint markers in such cases to the location of the true "
      "breakpoint."));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    item->setSettingsKey(debugModeGroup, "AdjustBreakpointLocations");
    insertItem(AdjustBreakpointLocations, item);

    item = new SavedAction;
    item->setText(tr("Break on \"throw\""));
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    item->setSettingsKey(debugModeGroup, "BreakOnThrow");
    insertItem(BreakOnThrow, item);

    item = new SavedAction;
    item->setText(tr("Break on \"catch\""));
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    item->setSettingsKey(debugModeGroup, "BreakOnCatch");
    insertItem(BreakOnCatch, item);

    item = new SavedAction;
    item->setText(tr("Break on \"qWarning\""));
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    item->setSettingsKey(debugModeGroup, "BreakOnWarning");
    insertItem(BreakOnWarning, item);

    item = new SavedAction;
    item->setText(tr("Break on \"qFatal\""));
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    item->setSettingsKey(debugModeGroup, "BreakOnFatal");
    insertItem(BreakOnFatal, item);

    item = new SavedAction;
    item->setText(tr("Break on \"abort\""));
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    item->setSettingsKey(debugModeGroup, "BreakOnAbort");
    insertItem(BreakOnAbort, item);

    //
    // Settings
    //

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "LoadGdbInit");
    item->setDefaultValue(QString());
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(LoadGdbInit, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "LoadGdbDumpers2");
    item->setDefaultValue(QString());
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    insertItem(LoadGdbDumpers, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "AutoEnrichParameters");
    item->setDefaultValue(QString());
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(AutoEnrichParameters, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "UseDynamicType");
    item->setText(tr("Use Dynamic Object Type for Display"));
    item->setCheckable(true);
    item->setDefaultValue(true);
    item->setValue(true);
    insertItem(UseDynamicType, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "TargetAsync");
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setValue(false);
    insertItem(TargetAsync, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "WarnOnReleaseBuilds");
    item->setCheckable(true);
    item->setDefaultValue(true);
    insertItem(WarnOnReleaseBuilds, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "GdbStartupCommands");
    item->setDefaultValue(QString());
    insertItem(GdbStartupCommands, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "GdbCustomDumperCommands");
    item->setDefaultValue(QString());
    insertItem(ExtraDumperCommands, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "ExtraDumperFile");
    item->setDefaultValue(QString());
    insertItem(ExtraDumperFile, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "GdbPostAttachCommands");
    item->setDefaultValue(QString());
    insertItem(GdbPostAttachCommands, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "CloseBuffersOnExit");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(CloseSourceBuffersOnExit, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "CloseMemoryBuffersOnExit");
    item->setCheckable(true);
    item->setDefaultValue(true);
    insertItem(CloseMemoryBuffersOnExit, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "SwitchModeOnExit");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(SwitchModeOnExit, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "BreakpointsFullPath");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(BreakpointsFullPathByDefault, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "RaiseOnInterrupt");
    item->setCheckable(true);
    item->setDefaultValue(true);
    insertItem(RaiseOnInterrupt, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "AutoQuit");
    item->setText(tr("Automatically Quit Debugger"));
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(AutoQuit, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "MultiInferior");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(MultiInferior, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "IntelFlavor");
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(IntelFlavor, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "UseToolTips");
    item->setText(tr("Use tooltips in main editor when debugging"));
    item->setToolTip(tr("<p>Checking this will enable tooltips for variable "
        "values during debugging. Since this can slow down debugging and "
        "does not provide reliable information as it does not use scope "
        "information, it is switched off by default."));
    item->setCheckable(true);
    item->setDefaultValue(true);
    insertItem(UseToolTipsInMainEditor, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "UseToolTipsInLocalsView");
    item->setText(tr("Use Tooltips in Locals View when Debugging"));
    item->setToolTip(tr("<p>Checking this will enable tooltips in the locals "
        "view during debugging."));
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(UseToolTipsInLocalsView, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "UseToolTipsInBreakpointsView");
    item->setText(tr("Use Tooltips in Breakpoints View when Debugging"));
    item->setToolTip(tr("<p>Checking this will enable tooltips in the breakpoints "
        "view during debugging."));
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(UseToolTipsInBreakpointsView, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "UseToolTipsInStackView");
    item->setText(tr("Use Tooltips in Stack View when Debugging"));
    item->setToolTip(tr("<p>Checking this will enable tooltips in the stack "
        "view during debugging."));
    item->setCheckable(true);
    item->setDefaultValue(true);
    insertItem(UseToolTipsInStackView, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "SkipKnownFrames");
    item->setText(tr("Skip Known Frames"));
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(SkipKnownFrames, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "EnableReverseDebugging");
    item->setText(tr("Enable Reverse Debugging"));
    item->setCheckable(true);
    item->setDefaultValue(false);
    item->setIcon(Icons::REVERSE_MODE.icon());
    insertItem(EnableReverseDebugging, item);

#ifdef Q_OS_WIN
    item = new RegisterPostMortemAction;
    item->setSettingsKey(debugModeGroup, "RegisterForPostMortem");
    item->setText(tr("Register For Post-Mortem Debugging"));
    item->setCheckable(true);
    item->setDefaultValue(false);
    insertItem(RegisterForPostMortem, item);
#endif

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "AllPluginBreakpoints");
    item->setDefaultValue(true);
    insertItem(AllPluginBreakpoints, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "SelectedPluginBreakpoints");
    item->setDefaultValue(false);
    insertItem(SelectedPluginBreakpoints, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "NoPluginBreakpoints");
    item->setDefaultValue(false);
    insertItem(NoPluginBreakpoints, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "SelectedPluginBreakpointsPattern");
    item->setDefaultValue(".*");
    insertItem(SelectedPluginBreakpointsPattern, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "MaximalStackDepth");
    item->setDefaultValue(20);
    insertItem(MaximalStackDepth, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "DisplayStringLimit");
    item->setToolTip(tr("<p>The maximum length of string entries in the "
        "Locals and Expressions views. Longer than that are cut off "
        "and displayed with an ellipsis attached."));
    item->setDefaultValue(100);
    insertItem(DisplayStringLimit, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "MaximalStringLength");
    item->setToolTip(tr("<p>The maximum length for strings in separated windows. "
        "Longer strings are cut off and displayed with an ellipsis attached."));
    item->setDefaultValue(10000);
    insertItem(MaximalStringLength, item);

    item = new SavedAction;
    item->setText(tr("Reload Full Stack"));
    insertItem(ExpandStack, item);

    item = new SavedAction;
    item->setText(tr("Create Full Backtrace"));
    insertItem(CreateFullBacktrace, item);

    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "WatchdogTimeout");
    item->setDefaultValue(20);
    insertItem(GdbWatchdogTimeout, item);

    //
    // QML Tools
    //
    item = new SavedAction;
    item->setSettingsKey(debugModeGroup, "ShowQmlObjectTree");
    item->setDefaultValue(true);
    insertItem(ShowQmlObjectTree, item);

    const QString qmlInspectorGroup = "QML.Inspector";
    item = new SavedAction;
    item->setSettingsKey(qmlInspectorGroup, "QmlInspector.ShowAppOnTop");
    item->setDefaultValue(false);
    insertItem(ShowAppOnTop, item);
}

DebuggerSettings::~DebuggerSettings()
{
    qDeleteAll(m_items);
}

void DebuggerSettings::insertItem(int code, SavedAction *item)
{
    QTC_ASSERT(!m_items.contains(code),
        qDebug() << code << item->toString(); return);
    QTC_ASSERT(item->settingsKey().isEmpty() || item->defaultValue().isValid(),
        qDebug() << "NO DEFAULT VALUE FOR " << item->settingsKey());
    m_items[code] = item;
}

void DebuggerSettings::readSettings()
{
    QSettings *settings = Core::ICore::settings();
    for (SavedAction *item : qAsConst(m_items))
        item->readSettings(settings);
}

void DebuggerSettings::writeSettings() const
{
    QSettings *settings = Core::ICore::settings();
    for (SavedAction *item : qAsConst(m_items))
        item->writeSettings(settings);
}

SavedAction *DebuggerSettings::item(int code) const
{
    QTC_ASSERT(m_items.value(code, nullptr), qDebug() << "CODE: " << code; return nullptr);
    return m_items.value(code, nullptr);
}

QString DebuggerSettings::dump()
{
    QStringList settings;
    for (SavedAction *item : qAsConst(theDebuggerSettings->m_items)) {
        QString key = item->settingsKey();
        if (!key.isEmpty()) {
            const QString current = item->value().toString();
            const QString default_ = item->defaultValue().toString();
            QString setting = key + ": " + current + "  (default: " + default_ + ')';
            if (current != default_)
                setting +=  "  ***";
            settings << setting;
        }
    }
    settings.sort();
    return "Debugger settings:\n" + settings.join('\n');
}

} // namespace Internal
} // namespace Debugger

