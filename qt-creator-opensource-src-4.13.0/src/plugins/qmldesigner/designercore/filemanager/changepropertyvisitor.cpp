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

#include "changepropertyvisitor.h"

#include <qmljs/parser/qmljsast_p.h>

using namespace QmlJS;
using namespace QmlJS::AST;
using namespace QmlDesigner;
using namespace QmlDesigner::Internal;

ChangePropertyVisitor::ChangePropertyVisitor(QmlDesigner::TextModifier &modifier,
                                             quint32 parentLocation,
                                             const QString &name,
                                             const QString &value,
                                             QmlRefactoring::PropertyType propertyType):
        QMLRewriter(modifier),
        m_parentLocation(parentLocation),
        m_name(name),
        m_value(value),
        m_propertyType(propertyType)
{
}

bool ChangePropertyVisitor::visit(QmlJS::AST::UiObjectDefinition *ast)
{
    if (didRewriting())
        return false;

    const quint32 objectStart = ast->firstSourceLocation().offset;

    if (objectStart == m_parentLocation) {
        // FIXME: change this to use the QmlJS::Rewriter class
        replaceInMembers(ast->initializer, m_name);
        return false;
    }

    return !didRewriting();
}

bool ChangePropertyVisitor::visit(QmlJS::AST::UiObjectBinding *ast)
{
    if (didRewriting())
        return false;

    const quint32 objectStart = ast->qualifiedTypeNameId->identifierToken.offset;

    if (objectStart == m_parentLocation) {
        // FIXME: change this to use the QmlJS::Rewriter class
        replaceInMembers(ast->initializer, m_name);
        return false;
    }

    return !didRewriting();
}

// FIXME: duplicate code in the QmlJS::Rewriter class, remove this
void ChangePropertyVisitor::replaceInMembers(UiObjectInitializer *initializer,
                                             const QString &propertyName)
{
    QString prefix, suffix;
    int dotIdx = propertyName.indexOf(QLatin1Char('.'));
    if (dotIdx != -1) {
        prefix = propertyName.left(dotIdx);
        suffix = propertyName.mid(dotIdx + 1);
    }

    for (UiObjectMemberList *members = initializer->members; members; members = members->next) {
        UiObjectMember *member = members->member;

        // for non-grouped properties:
        if (isMatchingPropertyMember(propertyName, member)) {
            switch (m_propertyType) {
            case QmlRefactoring::ArrayBinding:
                insertIntoArray(cast<UiArrayBinding*>(member));
                break;

            case QmlRefactoring::ObjectBinding:
                replaceMemberValue(member, false);
                break;

            case QmlRefactoring::ScriptBinding:
                replaceMemberValue(member, nextMemberOnSameLine(members));
                break;

            default:
                Q_ASSERT(!"Unhandled QmlRefactoring::PropertyType");
            }

            break;
        // for grouped properties:
        } else if (!prefix.isEmpty()) {
            if (auto def = cast<UiObjectDefinition *>(member)) {
                if (toString(def->qualifiedTypeNameId) == prefix)
                    replaceInMembers(def->initializer, suffix);
            }
        }
    }
}

// FIXME: duplicate code in the QmlJS::Rewriter class, remove this
void ChangePropertyVisitor::replaceMemberValue(UiObjectMember *propertyMember, bool needsSemicolon)
{
    QString replacement = m_value;
    int startOffset = -1;
    int endOffset = -1;
    if (auto objectBinding = AST::cast<UiObjectBinding *>(propertyMember)) {
        startOffset = objectBinding->qualifiedTypeNameId->identifierToken.offset;
        endOffset = objectBinding->initializer->rbraceToken.end();
    } else if (auto scriptBinding = AST::cast<UiScriptBinding *>(propertyMember)) {
        startOffset = scriptBinding->statement->firstSourceLocation().offset;
        endOffset = scriptBinding->statement->lastSourceLocation().end();
    } else if (auto arrayBinding = AST::cast<UiArrayBinding *>(propertyMember)) {
        startOffset = arrayBinding->lbracketToken.offset;
        endOffset = arrayBinding->rbracketToken.end();
    } else if (auto publicMember = AST::cast<UiPublicMember*>(propertyMember)) {
        if (publicMember->statement) {
            startOffset = publicMember->statement->firstSourceLocation().offset;
            if (publicMember->semicolonToken.isValid())
                endOffset = publicMember->semicolonToken.end();
            else
                endOffset = publicMember->statement->lastSourceLocation().end();
        } else {
            startOffset = publicMember->lastSourceLocation().end();
            endOffset = startOffset;
            if (publicMember->semicolonToken.isValid())
                startOffset = publicMember->semicolonToken.offset;
            replacement.prepend(QStringLiteral(": "));
        }
    } else {
        return;
    }

    if (needsSemicolon)
        replacement += QChar::fromLatin1(';');

    replace(startOffset, endOffset - startOffset, replacement);
    setDidRewriting(true);
}

// FIXME: duplicate code in the QmlJS::Rewriter class, remove this
bool ChangePropertyVisitor::isMatchingPropertyMember(const QString &propName,
                                                     UiObjectMember *member)
{
    if (auto objectBinding = AST::cast<UiObjectBinding *>(member))
        return propName == toString(objectBinding->qualifiedId);
    else if (auto scriptBinding = AST::cast<UiScriptBinding *>(member))
        return propName == toString(scriptBinding->qualifiedId);
    else if (auto arrayBinding = AST::cast<UiArrayBinding *>(member))
        return propName == toString(arrayBinding->qualifiedId);
    else if (auto publicMember = AST::cast<UiPublicMember *>(member))
        return propName == publicMember->name;
    else
        return false;
}

// FIXME: duplicate code in the QmlJS::Rewriter class, remove this
bool ChangePropertyVisitor::nextMemberOnSameLine(UiObjectMemberList *members)
{
    if (members && members->next && members->next->member)
        return members->next->member->firstSourceLocation().startLine == members->member->lastSourceLocation().startLine;
    else
        return false;
}

// FIXME: duplicate code in the QmlJS::Rewriter class, remove this
void ChangePropertyVisitor::insertIntoArray(QmlJS::AST::UiArrayBinding *ast)
{
    if (!ast)
        return;

    UiObjectMember *lastMember = nullptr;
    for (UiArrayMemberList *iter = ast->members; iter; iter = iter->next) {
        lastMember = iter->member;
    }

    if (!lastMember)
        return;

    const int insertionPoint = lastMember->lastSourceLocation().end();
    const int depth = calculateIndentDepth(lastMember->firstSourceLocation());
    const QString indentedArrayMember = addIndentation(m_value, depth);
    replace(insertionPoint, 0, QStringLiteral(",\n") + indentedArrayMember);
    setDidRewriting(true);
}
