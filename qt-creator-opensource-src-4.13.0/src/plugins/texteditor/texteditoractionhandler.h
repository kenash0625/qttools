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

#include "texteditor_global.h"

#include <utils/id.h>

#include <QObject>

#include <functional>

namespace Core {
class IEditor;
}

namespace TextEditor {
class TextEditorWidget;

namespace Internal { class TextEditorActionHandlerPrivate; }

// Redirects slots from global actions to the respective editor.

class TEXTEDITOR_EXPORT TextEditorActionHandler final
{
    TextEditorActionHandler(const TextEditorActionHandler &) = delete;
    TextEditorActionHandler &operator=(const TextEditorActionHandler &) = delete;

public:
    enum OptionalActionsMask {
        None = 0,
        Format = 1,
        UnCommentSelection = 2,
        UnCollapseAll = 4,
        FollowSymbolUnderCursor = 8,
        JumpToFileUnderCursor = 16,
        RenameSymbol = 32,
    };
    using TextEditorWidgetResolver = std::function<TextEditorWidget *(Core::IEditor *)>;

    TextEditorActionHandler(Utils::Id editorId,
                            Utils::Id contextId,
                            uint optionalActions = None,
                            const TextEditorWidgetResolver &resolver = {});

    uint optionalActions() const;
    ~TextEditorActionHandler();

private:
    Internal::TextEditorActionHandlerPrivate *d;
};

} // namespace TextEditor
