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

#include "abstractpackagingstep.h"

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/deploymentdata.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <projectexplorer/task.h>

#include <QDateTime>
#include <QFileInfo>

using namespace ProjectExplorer;

namespace RemoteLinux {
namespace Internal {

class AbstractPackagingStepPrivate
{
public:
    QString cachedPackageFilePath;
    QString cachedPackageDirectory;
    bool deploymentDataModified = false;
};

} // namespace Internal

AbstractPackagingStep::AbstractPackagingStep(BuildStepList *bsl, Utils::Id id)
    : BuildStep(bsl, id)
{
    d = new Internal::AbstractPackagingStepPrivate;

    connect(target(), &Target::deploymentDataChanged,
            this, &AbstractPackagingStep::setDeploymentDataModified);
    setDeploymentDataModified();

    connect(this, &AbstractPackagingStep::unmodifyDeploymentData,
            this, &AbstractPackagingStep::setDeploymentDataUnmodified);
}

AbstractPackagingStep::~AbstractPackagingStep()
{
    delete d;
}

QString AbstractPackagingStep::cachedPackageFilePath() const
{
    return d->cachedPackageFilePath;
}

QString AbstractPackagingStep::packageFilePath() const
{
    if (packageDirectory().isEmpty())
        return QString();
    return packageDirectory() + QLatin1Char('/') + packageFileName();
}

QString AbstractPackagingStep::cachedPackageDirectory() const
{
    return d->cachedPackageDirectory;
}

QString AbstractPackagingStep::packageDirectory() const
{
    return buildDirectory().toString();
}

bool AbstractPackagingStep::isPackagingNeeded() const
{
    QFileInfo packageInfo(packageFilePath());
    if (!packageInfo.exists() || d->deploymentDataModified)
        return true;

    const DeploymentData &dd = target()->deploymentData();
    for (int i = 0; i < dd.fileCount(); ++i) {
        if (dd.fileAt(i).localFilePath().isNewerThan(packageInfo.lastModified()))
            return true;
    }

    return false;
}

bool AbstractPackagingStep::init()
{
    d->cachedPackageDirectory = packageDirectory();
    d->cachedPackageFilePath = packageFilePath();
    return true;
}

void AbstractPackagingStep::setPackagingStarted()
{
}

// called in ::run thread
void AbstractPackagingStep::setPackagingFinished(bool success)
{
    if (success)
        emit unmodifyDeploymentData();
}

// called in gui thread
void AbstractPackagingStep::setDeploymentDataUnmodified()
{
    d->deploymentDataModified = false;
}

void AbstractPackagingStep::setDeploymentDataModified()
{
    d->deploymentDataModified = true;
}

void AbstractPackagingStep::raiseError(const QString &errorMessage)
{
    emit addTask(DeploymentTask(Task::Error, errorMessage));
    emit addOutput(errorMessage, BuildStep::OutputFormat::Stderr);
}

void AbstractPackagingStep::raiseWarning(const QString &warningMessage)
{
    emit addTask(DeploymentTask(Task::Warning, warningMessage));
    emit addOutput(warningMessage, OutputFormat::ErrorMessage);
}

} // namespace RemoteLinux
