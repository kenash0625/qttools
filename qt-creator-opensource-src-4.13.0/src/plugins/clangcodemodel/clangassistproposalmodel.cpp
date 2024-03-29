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

#include "clangassistproposalitem.h"

#include "clangassistproposalmodel.h"

#include <texteditor/codeassist/assistproposaliteminterface.h>

#include <algorithm>

namespace ClangCodeModel {
namespace Internal {

constexpr int SORT_LIMIT = 30000;

bool ClangAssistProposalModel::containsDuplicates() const
{
    return false;
}

bool ClangAssistProposalModel::isSortable(const QString &/*prefix*/) const
{
    return m_currentItems.size() <= SORT_LIMIT;
}

void ClangAssistProposalModel::sort(const QString &/*prefix*/)
{
    using TextEditor::AssistProposalItemInterface;

    auto currentItemsCompare = [](AssistProposalItemInterface *first,
                                  AssistProposalItemInterface *second) {
        if (first->proposalMatch() != second->proposalMatch()) {
            return static_cast<int>(first->proposalMatch())
                    < static_cast<int>(second->proposalMatch());
        }
        return false;
    };

    // Keep the order for the items with the same priority and name.
    std::stable_sort(m_currentItems.begin(), m_currentItems.end(), currentItemsCompare);
}

} // namespace Internal
} // namespace ClangCodeModel

