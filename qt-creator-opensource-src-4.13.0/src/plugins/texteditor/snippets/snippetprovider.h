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

#include <texteditor/texteditor_global.h>

#include <QString>

#include <functional>

namespace TextEditor {

class TextEditorWidget;

class TEXTEDITOR_EXPORT SnippetProvider
{
public:
    SnippetProvider() = default;

    using EditorDecorator = std::function<void(TextEditorWidget *)>;
    using EditorDecorator2 = std::function<void(TextEditorWidget *,void*)>;

    static const QList<SnippetProvider> &snippetProviders();
    static void registerGroup(const QString &groupId, const QString &displayName,
                              EditorDecorator editorDecorator = EditorDecorator());
    static void registerGroup2(const QString &groupId, const QString &displayName,
                              EditorDecorator2 editorDecorator = EditorDecorator2());

    QString groupId() const;
    QString displayName() const;

    static void decorateEditor(TextEditorWidget *editor, const QString &groupId);
    static void decorateEditor2(TextEditorWidget *editor, const QString &groupId,void*);

private:
    QString m_groupId;
    QString m_displayName;
    EditorDecorator m_editorDecorator;
    EditorDecorator2 m_editorDecorator2;
};

} // TextEditor
