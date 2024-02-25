/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//


#include <QAbstractTableModel>
#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>
#include <vector>
#include "ui_plugindialog.h"
#include "TypeofPluginFunction"
QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

namespace qdesigner_internal {

class PluginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PluginDialog(QDesignerFormEditorInterface *core, QWidget *parent = nullptr);

private slots:
    void updateCustomWidgetPlugins();

private:
    void populateTreeWidget();
    QTreeWidgetItem* setTopLevelItem(const QString &itemName);
    QTreeWidgetItem* setPluginItem(QTreeWidgetItem *topLevelItem,
                                   const QString &itemName, const QFont &font);
    void setItem(QTreeWidgetItem *pluginItem, const QString &name,
                 const QString &toolTip, const QString &whatsThis, const QIcon &icon);

    QDesignerFormEditorInterface *m_core;
    Ui::PluginDialog ui;
    QIcon interfaceIcon;
    QIcon featureIcon;
};

}

QT_END_NAMESPACE
#include <QDialog>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
QT_END_NAMESPACE

namespace ExtensionSystem {
class PluginSpec;
class PluginView;
}

namespace Core {
namespace Internal {

class PluginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginDialog(QDesignerFormEditorInterface *core, QWidget *parent);

private:
    void updateRestartRequired(ExtensionSystem::PluginSpec *spec);
    void updateButtons();
    void openDetails(ExtensionSystem::PluginSpec *spec);
    void openErrorDetails();
    void closeDialog();
    void showInstallWizard();
    void updateCustomWidgetPlugins();

    ExtensionSystem::PluginView *m_view;
    QDesignerFormEditorInterface *m_core;

    QPushButton *m_detailsButton;
    QPushButton *m_errorDetailsButton;
    QPushButton *m_installButton;
    QPushButton *m_closeButton;
    QLabel *m_restartRequired;
};



class ModelPluginFunction : public QAbstractTableModel
{
public:
    QList<TypeofPluginFunction> mdldata;
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
} // namespace Internal
} // namespace Core

#endif
