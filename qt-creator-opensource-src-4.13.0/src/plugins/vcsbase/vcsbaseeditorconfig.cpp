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

#include "vcsbaseeditorconfig.h"

#include <utils/utilsicons.h>

#include <QComboBox>
#include <QAction>
#include <QHBoxLayout>

#include <QStringList>
#include <QDebug>

namespace VcsBase {

namespace Internal {

class SettingMappingData
{
public:
    enum Type
    {
        Invalid,
        Bool,
        String,
        Int
    };

    SettingMappingData() : boolSetting(nullptr)
    { }

    SettingMappingData(bool *setting) : boolSetting(setting), m_type(Bool)
    { }

    SettingMappingData(QString *setting) : stringSetting(setting), m_type(String)
    { }

    SettingMappingData(int *setting) : intSetting(setting), m_type(Int)
    { }

    Type type() const
    {
        return m_type;
    }

    union {
        bool *boolSetting;
        QString *stringSetting;
        int *intSetting;
    };

private:
    Type m_type = Invalid;
};

class VcsBaseEditorConfigPrivate
{
public:
    VcsBaseEditorConfigPrivate(QToolBar *toolBar) : m_toolBar(toolBar)
    {
        if (!toolBar)
            return;
        toolBar->setContentsMargins(3, 0, 3, 0);
        toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }

    QStringList m_baseArguments;
    QList<VcsBaseEditorConfig::OptionMapping> m_optionMappings;
    QHash<QObject *, SettingMappingData> m_settingMapping;
    QToolBar *m_toolBar;
};

} // namespace Internal

/*!
    \class VcsBase::VcsBaseEditorConfig

    \brief The VcsBaseEditorConfig is a widget/action aggregator for use
    with VcsBase::VcsBaseEditor, influencing for example the generation of VCS diff output.

    The class maintains a list of command line arguments (starting from baseArguments())
    which are set according to the state of the inside widgets. A change signal is provided
    that should trigger the rerun of the VCS operation.
*/

VcsBaseEditorConfig::ChoiceItem::ChoiceItem(const QString &text, const QVariant &val) :
    displayText(text),
    value(val)
{
}

VcsBaseEditorConfig::VcsBaseEditorConfig(QToolBar *toolBar) :
    QObject(toolBar), d(new Internal::VcsBaseEditorConfigPrivate(toolBar))
{
    connect(this, &VcsBaseEditorConfig::argumentsChanged,
            this, &VcsBaseEditorConfig::handleArgumentsChanged);
}

VcsBaseEditorConfig::~VcsBaseEditorConfig()
{
    delete d;
}

QStringList VcsBaseEditorConfig::baseArguments() const
{
    return d->m_baseArguments;
}

void VcsBaseEditorConfig::setBaseArguments(const QStringList &b)
{
    d->m_baseArguments = b;
}

QAction *VcsBaseEditorConfig::addReloadButton()
{
    auto action = new QAction(Utils::Icons::RELOAD_TOOLBAR.icon(), tr("Reload"), d->m_toolBar);
    connect(action, &QAction::triggered, this, &VcsBaseEditorConfig::argumentsChanged);
    addAction(action);
    return action;
}

QStringList VcsBaseEditorConfig::arguments() const
{
    // Compile effective arguments
    QStringList args = baseArguments();
    foreach (const OptionMapping &mapping, optionMappings())
        args += argumentsForOption(mapping);
    return args;
}

QAction *VcsBaseEditorConfig::addToggleButton(const QString &option,
                                              const QString &label,
                                              const QString &tooltip)
{
    return addToggleButton(option.isEmpty() ? QStringList() : QStringList(option), label, tooltip);
}

QAction *VcsBaseEditorConfig::addToggleButton(const QStringList &options,
                                              const QString &label,
                                              const QString &tooltip)
{
    auto action = new QAction(label, d->m_toolBar);
    action->setToolTip(tooltip);
    action->setCheckable(true);
    connect(action, &QAction::toggled, this, &VcsBaseEditorConfig::argumentsChanged);
    addAction(action);
    d->m_optionMappings.append(OptionMapping(options, action));
    return action;
}

QComboBox *VcsBaseEditorConfig::addChoices(const QString &title,
                                           const QStringList &options,
                                           const QList<ChoiceItem> &items)
{
    auto cb = new QComboBox;
    cb->setToolTip(title);
    for (const ChoiceItem &item : items)
        cb->addItem(item.displayText, item.value);
    connect(cb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VcsBaseEditorConfig::argumentsChanged);
    d->m_toolBar->addWidget(cb);
    d->m_optionMappings.append(OptionMapping(options, cb));
    return cb;
}

void VcsBaseEditorConfig::mapSetting(QAction *button, bool *setting)
{
    if (!d->m_settingMapping.contains(button) && button) {
        d->m_settingMapping.insert(button, Internal::SettingMappingData(setting));
        if (setting) {
            QSignalBlocker blocker(button);
            button->setChecked(*setting);
        }
    }
}

void VcsBaseEditorConfig::mapSetting(QComboBox *comboBox, QString *setting)
{
    if (!d->m_settingMapping.contains(comboBox) && comboBox) {
        d->m_settingMapping.insert(comboBox, Internal::SettingMappingData(setting));
        if (setting) {
            QSignalBlocker blocker(comboBox);
            const int itemIndex = comboBox->findData(*setting);
            if (itemIndex != -1)
                comboBox->setCurrentIndex(itemIndex);
        }
    }
}

void VcsBaseEditorConfig::mapSetting(QComboBox *comboBox, int *setting)
{
    if (d->m_settingMapping.contains(comboBox) || !comboBox)
        return;

    d->m_settingMapping.insert(comboBox, Internal::SettingMappingData(setting));

    if (!setting || *setting < 0 || *setting >= comboBox->count())
        return;

    QSignalBlocker blocker(comboBox);
    comboBox->setCurrentIndex(*setting);
}

void VcsBaseEditorConfig::handleArgumentsChanged()
{
    updateMappedSettings();
    executeCommand();
}

void VcsBaseEditorConfig::executeCommand()
{
    emit commandExecutionRequested();
}

VcsBaseEditorConfig::OptionMapping::OptionMapping(const QString &option, QObject *obj) :
    object(obj)
{
    if (!option.isEmpty())
        options << option;
}

VcsBaseEditorConfig::OptionMapping::OptionMapping(const QStringList &optionList, QObject *obj) :
    options(optionList),
    object(obj)
{
}

const QList<VcsBaseEditorConfig::OptionMapping> &VcsBaseEditorConfig::optionMappings() const
{
    return d->m_optionMappings;
}

QStringList VcsBaseEditorConfig::argumentsForOption(const OptionMapping &mapping) const
{
    auto action = qobject_cast<const QAction *>(mapping.object);
    if (action && action->isChecked())
        return mapping.options;

    QStringList args;
    auto cb = qobject_cast<const QComboBox *>(mapping.object);
    if (!cb)
        return args;

    const QString value = cb->itemData(cb->currentIndex()).toString();
    if (value.isEmpty())
        return args;

    if (mapping.options.isEmpty())
        args += value.split(' ');
    else
        args += mapping.options.first().arg(value);
    return args;
}

void VcsBaseEditorConfig::updateMappedSettings()
{
    foreach (const OptionMapping &optMapping, d->m_optionMappings) {
        if (d->m_settingMapping.contains(optMapping.object)) {
            Internal::SettingMappingData& settingData = d->m_settingMapping[optMapping.object];
            switch (settingData.type()) {
            case Internal::SettingMappingData::Bool :
            {
                if (auto action = qobject_cast<const QAction *>(optMapping.object))
                    *settingData.boolSetting = action->isChecked();
                break;
            }
            case Internal::SettingMappingData::String :
            {
                auto cb = qobject_cast<const QComboBox *>(optMapping.object);
                if (cb && cb->currentIndex() != -1)
                    *settingData.stringSetting = cb->itemData(cb->currentIndex()).toString();
                break;
            }
            case Internal::SettingMappingData::Int:
            {
                auto cb = qobject_cast<const QComboBox *>(optMapping.object);
                if (cb && cb->currentIndex() != -1)
                    *settingData.intSetting = cb->currentIndex();
                break;
            }
            case Internal::SettingMappingData::Invalid : break;
            } // end switch ()
        }
    }
}

void VcsBaseEditorConfig::addAction(QAction *action)
{
    d->m_toolBar->addAction(action);
}

} // namespace VcsBase
