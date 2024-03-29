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

#include <coreplugin/dialogs/ioptionspage.h>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace CppTools {
namespace Internal {

struct CppFileSettings
{
    QStringList headerPrefixes;
    QString headerSuffix;
    QStringList headerSearchPaths;
    QStringList sourcePrefixes;
    QString sourceSuffix;
    QStringList sourceSearchPaths;
    QString licenseTemplatePath;
    bool headerPragmaOnce = false;
    bool lowerCaseFiles = false;

    void toSettings(QSettings *) const;
    void fromSettings(QSettings *);
    bool applySuffixesToMimeDB();

    // Convenience to return a license template completely formatted.
    // Currently made public in
    static QString licenseTemplate();

    bool equals(const CppFileSettings &rhs) const;
    bool operator==(const CppFileSettings &s) const { return equals(s); }
    bool operator!=(const CppFileSettings &s) const { return !equals(s); }
};

class CppFileSettingsPage : public Core::IOptionsPage
{
public:
    explicit CppFileSettingsPage(CppFileSettings *settings);
};

} // namespace Internal
} // namespace CppTools
