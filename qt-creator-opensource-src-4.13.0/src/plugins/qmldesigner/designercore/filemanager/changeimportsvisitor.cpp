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

#include "changeimportsvisitor.h"

#include <qmljs/parser/qmljsast_p.h>

using namespace QmlDesigner;
using namespace QmlDesigner::Internal;

ChangeImportsVisitor::ChangeImportsVisitor(TextModifier &textModifier,
                                           const QString &source):
        QMLRewriter(textModifier), m_source(source)
{}

bool ChangeImportsVisitor::add(QmlJS::AST::UiProgram *ast, const Import &import)
{
    setDidRewriting(false);
    if (!ast)
        return false;

    if (ast->headers && ast->headers->headerItem) {
        int insertionPoint = 0;
        if (ast->members && ast->members->member)
            insertionPoint = ast->members->member->firstSourceLocation().begin();
        else
            insertionPoint = m_source.length();
        while (insertionPoint > 0) {
            --insertionPoint;
            const QChar c = m_source.at(insertionPoint);
            if (!c.isSpace() && c != QLatin1Char(';'))
                break;
        }
        replace(insertionPoint+1, 0, QStringLiteral("\n") + import.toImportString());
    } else {
        replace(0, 0, import.toImportString() + QStringLiteral("\n\n"));
    }

    setDidRewriting(true);

    return true;
}

bool ChangeImportsVisitor::remove(QmlJS::AST::UiProgram *ast, const Import &import)
{
    setDidRewriting(false);
    if (!ast)
        return false;

    for (QmlJS::AST::UiHeaderItemList *iter = ast->headers; iter; iter = iter->next) {
        auto iterImport = QmlJS::AST::cast<QmlJS::AST::UiImport *>(iter->headerItem);
        if (equals(iterImport, import)) {
            int start = iterImport->firstSourceLocation().begin();
            int end = iterImport->lastSourceLocation().end();
            includeSurroundingWhitespace(start, end);
            replace(start, end - start, QString());
            setDidRewriting(true);
        }
    }

    return didRewriting();
}

bool ChangeImportsVisitor::equals(QmlJS::AST::UiImport *ast, const Import &import)
{
    bool equal = false;
    if (import.isLibraryImport())
        equal = toString(ast->importUri) == import.url();
    else if (import.isFileImport())
        equal = ast->fileName == import.file();

    if (equal) {
        equal = (!ast->version || (ast->version->minorVersion == 0 && ast->version->majorVersion == 0))
                && import.version().isEmpty();
        if (!equal && ast->version) {
            const QStringList versions = import.version().split('.');
            if (versions.size() >= 1 && versions[0].toInt() == ast->version->majorVersion) {
                if (versions.size() >= 2)
                    equal = versions[1].toInt() == ast->version->minorVersion;
                else
                    equal = ast->version->minorVersion == 0;
            }
        }
    }

    return equal;
}
