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

#include "qmakeprojectmanager_global.h"

#include <projectexplorer/makestep.h>

namespace QmakeProjectManager {
namespace Internal {

class QmakeMakeStepFactory : public ProjectExplorer::BuildStepFactory
{
public:
    QmakeMakeStepFactory();
};

} //namespace Internal

class QmakeProject;

class QMAKEPROJECTMANAGER_EXPORT QmakeMakeStep : public ProjectExplorer::MakeStep
{
    Q_OBJECT

public:
    QmakeMakeStep(ProjectExplorer::BuildStepList *bsl, Utils::Id id);

private:
    void finish(bool success) override;
    bool init() override;
    void setupOutputFormatter(Utils::OutputFormatter *formatter) override;
    void doRun() override;
    QStringList displayArguments() const override;

    bool m_scriptTarget = false;
    QString m_makeFileToCheck;
    bool m_unalignedBuildDir;
    bool m_ignoredNonTopLevelBuild = false;
};

} // QmakeProjectManager
