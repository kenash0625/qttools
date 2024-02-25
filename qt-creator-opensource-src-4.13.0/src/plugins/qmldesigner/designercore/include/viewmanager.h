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

#include "abstractview.h"

#include <QWidgetAction>

#include <utils/fileutils.h>

namespace ProjectExplorer {
class Target;
}

namespace QmlDesigner {

class DesignDocument;
class AbstractCustomTool;
class DesignerActionManager;
class NodeInstanceView;
class RewriterView;
class Edit3DView;

namespace Internal { class DesignModeWidget; }

class ViewManagerData;

class QMLDESIGNERCORE_EXPORT ViewManager
{
public:
    ViewManager();
    ~ViewManager();

    void attachRewriterView();
    void detachRewriterView();

    void attachComponentView();
    void detachComponentView();

    void attachViewsExceptRewriterAndComponetView();
    void detachViewsExceptRewriterAndComponetView();

    void setItemLibraryViewResourcePath(const QString &resourcePath);
    void setComponentNode(const ModelNode &componentNode);
    void setComponentViewToMaster();
    void setNodeInstanceViewTarget(ProjectExplorer::Target *target);

    void resetPropertyEditorView();

    void registerFormEditorToolTakingOwnership(AbstractCustomTool *tool);
    void registerViewTakingOwnership(AbstractView *view);

    QList<WidgetInfo> widgetInfos() const;
    QWidget *widget(const QString & uniqueId) const;

    void disableWidgets();
    void enableWidgets();

    void pushFileOnCrumbleBar(const Utils::FilePath &fileName);
    void pushInFileComponentOnCrumbleBar(const ModelNode &modelNode);
    void nextFileIsCalledInternally();

    NodeInstanceView *nodeInstanceView() const;

    void exportAsImage();
    void reformatFileUsingTextEditorView();

    QWidgetAction *componentViewAction() const;

    DesignerActionManager &designerActionManager();
    const DesignerActionManager &designerActionManager() const;

    void toggleStatesViewExpanded();

    void qmlJSEditorContextHelp(const Core::IContext::HelpCallback &callback) const;
    DesignDocument *currentDesignDocument() const;

    bool usesRewriterView(RewriterView *rewriterView);

    void disableStandardViews();
    void enableStandardViews();

private: // functions
    Q_DISABLE_COPY(ViewManager)

    void attachNodeInstanceView();
    void attachItemLibraryView();
    void attachAdditionalViews();
    void detachAdditionalViews();
    void detachStandardViews();

    Model *currentModel() const;
    Model *documentModel() const;
    QString pathToQt() const;

    void switchStateEditorViewToBaseState();
    void switchStateEditorViewToSavedState();
    QList<QPointer<AbstractView>> views() const;
    const QList<QPointer<AbstractView> > standardViews() const;

private: // variables
    std::unique_ptr<ViewManagerData> d;
};

} // namespace QmlDesigner
