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

#include "../testconfiguration.h"

namespace Autotest {
namespace Internal {

class QuickTestConfiguration : public DebuggableTestConfiguration
{
public:
    explicit QuickTestConfiguration(ITestFramework *framework);
    TestOutputReader *outputReader(const QFutureInterface<TestResultPtr> &fi,
                                   QProcess *app) const override;
    QStringList argumentsForTestRunner(QStringList *omitted = nullptr) const override;
    Utils::Environment filteredEnvironment(const Utils::Environment &original) const override;
    void setUnnamedOnly(bool unnamedOnly);
    bool unnamedOnly() const { return m_unnamedOnly; }

private:
    bool m_unnamedOnly = false;
};

} // namespace Internal
} // namespace Autotest
