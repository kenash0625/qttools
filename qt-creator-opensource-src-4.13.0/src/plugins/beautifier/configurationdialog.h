/****************************************************************************
**
** Copyright (C) 2016 Lorenz Haas
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

#include <QDialog>
#include <QString>

namespace Beautifier {
namespace Internal {

class AbstractSettings;
namespace Ui { class ConfigurationDialog; }

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent = nullptr);
    ~ConfigurationDialog() override;
    void setSettings(AbstractSettings *settings);

    void clear();
    QString key() const;
    void setKey(const QString &key);
    QString value() const;

private:
    void updateOkButton();
    void updateDocumentation(const QString &word = QString(), const QString &docu = QString());
    Ui::ConfigurationDialog *ui;
    AbstractSettings *m_settings = nullptr;
    QString m_currentKey;
};


} // namespace Internal
} // namespace Beautifier
