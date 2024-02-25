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

#include "cppmodelmanagersupport.h"

#include <QScopedPointer>

namespace CppTools {
namespace Internal {

class BuiltinModelManagerSupport: public ModelManagerSupport
{
    Q_DISABLE_COPY(BuiltinModelManagerSupport)

public:
    BuiltinModelManagerSupport();
    ~BuiltinModelManagerSupport() override;

    CppCompletionAssistProvider *completionAssistProvider() final;
    TextEditor::BaseHoverHandler *createHoverHandler() final;
    BaseEditorDocumentProcessor *createEditorDocumentProcessor(
            TextEditor::TextDocument *baseTextDocument) final;
    FollowSymbolInterface &followSymbolInterface() final;
    RefactoringEngineInterface &refactoringEngineInterface() final;
    std::unique_ptr<AbstractOverviewModel> createOverviewModel() final;

private:
    QScopedPointer<CppCompletionAssistProvider> m_completionAssistProvider;
    QScopedPointer<FollowSymbolInterface> m_followSymbol;
    QScopedPointer<RefactoringEngineInterface> m_refactoringEngine;
};

class BuiltinModelManagerSupportProvider : public ModelManagerSupportProvider
{
public:
    QString id() const override;
    QString displayName() const override;

    CppTools::ModelManagerSupport::Ptr createModelManagerSupport() override;
};

} // Internal namespace
} // CppTools namespace
