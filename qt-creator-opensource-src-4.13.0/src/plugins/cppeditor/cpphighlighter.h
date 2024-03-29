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

#include "cppeditor_global.h"

#include <texteditor/syntaxhighlighter.h>

#include <cplusplus/Token.h>

#include <QTextCharFormat>

namespace CppEditor {

class CPPEDITOR_EXPORT CppHighlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    CppHighlighter(void*poSkipHighlight=nullptr,QTextDocument *document = nullptr);

    void setLanguageFeatures(const CPlusPlus::LanguageFeatures &languageFeatures);
    void highlightBlock(const QString &text) override;
    void *poSkipHighlight;

private:
    void highlightWord(QStringRef word, int position, int length);
    bool highlightRawStringLiteral(const QStringView &text, const CPlusPlus::Token &tk);

    void highlightDoxygenComment(const QString &text, int position,
                                 int length);

    bool isPPKeyword(const QStringRef &text) const;

private:
    CPlusPlus::LanguageFeatures m_languageFeatures = CPlusPlus::LanguageFeatures::defaultFeatures();

};

} // namespace CppEditor
