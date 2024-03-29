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

#pragma once

#include "buildstep.h"
#include "projectexplorersettings.h"
#include <coreplugin/dialogs/ioptionspage.h>
#include <coreplugin/ioutputpane.h>

#include <QHash>
#include <QPair>

QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE

namespace Core { class OutputWindow; }
namespace Utils { class OutputFormatter; }

namespace ProjectExplorer {
class Task;

namespace Internal {
class ShowOutputTaskHandler;
class CompileOutputTextEdit;

class CompileOutputWindow : public Core::IOutputPane
{
    Q_OBJECT

public:
    explicit CompileOutputWindow(QAction *cancelBuildAction);
    ~CompileOutputWindow() override;

    QWidget *outputWidget(QWidget *) override;
    QList<QWidget *> toolBarWidgets() const override;
    QString displayName() const override { return tr("Compile Output"); }
    int priorityInStatusBar() const override;
    void clearContents() override;
    void visibilityChanged(bool visible) override;
    bool canFocus() const override;
    bool hasFocus() const override;
    void setFocus() override;

    bool canNext() const override;
    bool canPrevious() const override;
    void goToNext() override;
    void goToPrev() override;
    bool canNavigate() const override;

    void appendText(const QString &text, BuildStep::OutputFormat format);

    void registerPositionOf(const Task &task, int linkedOutputLines, int skipLines, int offset = 0);
    bool knowsPositionOf(const Task &task);
    void showPositionOf(const Task &task);

    void flush();
    void reset();

    const CompileOutputSettings &settings() const { return m_settings; }
    void setSettings(const CompileOutputSettings &settings);

    Utils::OutputFormatter *outputFormatter() const;

private:
    void updateFilter() override;

    void loadSettings();
    void storeSettings() const;
    void updateFromSettings();

    Core::OutputWindow *m_outputWindow;
    QHash<unsigned int, QPair<int, int>> m_taskPositions;
    ShowOutputTaskHandler *m_handler;
    QToolButton *m_cancelBuildButton;
    QToolButton * const m_settingsButton;
    CompileOutputSettings m_settings;
};

class CompileOutputSettingsPage final : public Core::IOptionsPage
{
public:
    CompileOutputSettingsPage();
};

} // namespace Internal
} // namespace ProjectExplorer
