/****************************************************************************
**
** Copyright (C) 2018 Sergey Morozov
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

#include <utils/fileutils.h>

namespace Cppcheck {
namespace Internal {

class Diagnostic final
{
public:
    enum class Severity {
        Error, Warning, Performance, Portability, Style, Information
    };
    bool isValid() const {
        return !fileName.isEmpty() && lineNumber > 0;
    }
    bool operator==(const Diagnostic& diagnostic) const;

    Severity severity = Severity::Information;
    QString severityText;
    QString checkId;
    QString message;
    Utils::FilePath fileName;
    int lineNumber = 0;
};

quint32 qHash(const Diagnostic &diagnostic);

} // namespace Internal
} // namespace Cppcheck
