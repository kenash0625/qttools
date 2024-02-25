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


#include "plugindialog_p.h"
#include "pluginmanager_p.h"
#include "iconloader_p.h"

#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/abstractintegration.h>
#include <QtDesigner/abstractwidgetdatabase.h>
#include <QtDesigner/QDesignerWidgetBoxInterface>

#include <QtUiPlugin/customwidget.h>

#include <QtWidgets/qstyle.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qpushbutton.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qpluginloader.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

PluginDialog::PluginDialog(QDesignerFormEditorInterface *core, QWidget *parent)
    : QDialog(parent
#ifdef Q_OS_MACOS
            , Qt::Tool
#endif
            ), m_core(core)
{
    ui.setupUi(this);

    ui.message->hide();

    const QStringList headerLabels(tr("Components"));

    ui.treeWidget->setAlternatingRowColors(false);
    ui.treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui.treeWidget->setHeaderLabels(headerLabels);
    ui.treeWidget->header()->hide();

    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                            QIcon::Normal, QIcon::On);
    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                            QIcon::Normal, QIcon::Off);
    featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    setWindowTitle(tr("Plugin Information"));
    populateTreeWidget();

    QPushButton *updateButton = new QPushButton(tr("Refresh"));
    const QString tooltip = tr("Scan for newly installed custom widget plugins.");
    updateButton->setToolTip(tooltip);
    updateButton->setWhatsThis(tooltip);
    connect(updateButton, &QAbstractButton::clicked, this, &PluginDialog::updateCustomWidgetPlugins);
    ui.buttonBox->addButton(updateButton, QDialogButtonBox::ActionRole);

}

void PluginDialog::populateTreeWidget()
{
    ui.treeWidget->clear();
    QDesignerPluginManager *pluginManager = m_core->pluginManager();
    const QStringList fileNames = pluginManager->registeredPlugins();

    if (!fileNames.isEmpty()) {
        QTreeWidgetItem *topLevelItem = setTopLevelItem(tr("Loaded Plugins"));
        QFont boldFont = topLevelItem->font(0);

        for (const QString &fileName : fileNames) {
            QPluginLoader loader(fileName);
            const QFileInfo fileInfo(fileName);

            QTreeWidgetItem *pluginItem = setPluginItem(topLevelItem, fileInfo.fileName(), boldFont);

            if (QObject *plugin = loader.instance()) {
                if (const QDesignerCustomWidgetCollectionInterface *c = qobject_cast<QDesignerCustomWidgetCollectionInterface*>(plugin)) {
                    const auto &collCustomWidgets = c->customWidgets();
                    for (const QDesignerCustomWidgetInterface *p : collCustomWidgets)
                        setItem(pluginItem, p->name(), p->toolTip(), p->whatsThis(), p->icon());
                } else {
                    if (const QDesignerCustomWidgetInterface *p = qobject_cast<QDesignerCustomWidgetInterface*>(plugin))
                        setItem(pluginItem, p->name(), p->toolTip(), p->whatsThis(), p->icon());
                }
            }
        }
    }

    const QStringList notLoadedPlugins = pluginManager->failedPlugins();
    if (!notLoadedPlugins.isEmpty()) {
        QTreeWidgetItem *topLevelItem = setTopLevelItem(tr("Failed Plugins"));
        const QFont boldFont = topLevelItem->font(0);
        for (const QString &plugin : notLoadedPlugins) {
            const QString failureReason = pluginManager->failureReason(plugin);
            QTreeWidgetItem *pluginItem = setPluginItem(topLevelItem, plugin, boldFont);
            setItem(pluginItem, failureReason, failureReason, QString(), QIcon());
        }
    }

    if (ui.treeWidget->topLevelItemCount() == 0) {
        ui.label->setText(tr("Qt Designer couldn't find any plugins"));
        ui.treeWidget->hide();
    } else {
        ui.label->setText(tr("Qt Designer found the following plugins"));
    }
}

QTreeWidgetItem* PluginDialog::setTopLevelItem(const QString &itemName)
{
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(ui.treeWidget);
    topLevelItem->setText(0, itemName);
    topLevelItem->setExpanded(true);
    topLevelItem->setIcon(0, style()->standardPixmap(QStyle::SP_DirOpenIcon));

    QFont boldFont = topLevelItem->font(0);
    boldFont.setBold(true);
    topLevelItem->setFont(0, boldFont);

    return topLevelItem;
}

QTreeWidgetItem* PluginDialog::setPluginItem(QTreeWidgetItem *topLevelItem,
                                             const QString &itemName, const QFont &font)
{
    QTreeWidgetItem *pluginItem = new QTreeWidgetItem(topLevelItem);
    pluginItem->setFont(0, font);
    pluginItem->setText(0, itemName);
    pluginItem->setExpanded(true);
    pluginItem->setIcon(0, style()->standardPixmap(QStyle::SP_DirOpenIcon));

    return pluginItem;
}

void PluginDialog::setItem(QTreeWidgetItem *pluginItem, const QString &name,
                           const QString &toolTip, const QString &whatsThis, const QIcon &icon)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(pluginItem);
    item->setText(0, name);
    item->setToolTip(0, toolTip);
    item->setWhatsThis(0, whatsThis);
    item->setIcon(0, icon.isNull() ? qtLogoIcon() : icon);
}

void  PluginDialog::updateCustomWidgetPlugins()
{
    const int before = m_core->widgetDataBase()->count();
    m_core->integration()->updateCustomWidgetPlugins();
    const int after = m_core->widgetDataBase()->count();
    if (after >  before) {
        ui.message->setText(tr("New custom widget plugins have been found."));
        ui.message->show();
    } else {
        ui.message->setText(QString());
    }
    populateTreeWidget();
}

}

QT_END_NAMESPACE

#include <extensionsystem/plugindetailsview.h>
#include <extensionsystem/pluginerrorview.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>
#include <extensionsystem/pluginview.h>

#include <utils/fancylineedit.h>
#include <utils/theme/theme_p.h>
#include <QCheckBox>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <customwidget.h>
#include <QMetaMethod>
#include <QTableView>
#include <QDir>
#include <plugininstallwizard.h>
using namespace Utils;

QT_BEGIN_NAMESPACE

namespace Core {
namespace Internal {

static bool s_isRestartRequired = false;

PluginDialog::PluginDialog(QDesignerFormEditorInterface *core, QWidget *parent)
    : QDialog(parent),m_core(core)
{
    ExtensionSystem::PluginManager::setPluginIID(QLatin1String("org.qt-project.Qt.QtCreatorPlugin"));
    const QString strpath=QCoreApplication::applicationDirPath()+"/../plugins/designer";
    ExtensionSystem::PluginManager::setPluginPaths({ strpath });
    m_view=(new ExtensionSystem::PluginView(this));
    auto vl = new QVBoxLayout(this);

    auto filterLayout = new QHBoxLayout;
    vl->addLayout(filterLayout);
    auto filterEdit = new Utils::FancyLineEdit(this);
    filterEdit->setFiltering(true);
    connect(filterEdit, &Utils::FancyLineEdit::filterChanged,
            m_view, &ExtensionSystem::PluginView::setFilter);
    filterLayout->addWidget(filterEdit);

    vl->addWidget(m_view);

    m_detailsButton = new QPushButton(tr("Details"), this);
    m_errorDetailsButton = new QPushButton(tr("Error Details"), this);
    m_closeButton = new QPushButton(tr("Close"), this);
    m_installButton = new QPushButton(tr("Install Plugin..."), this);
    m_detailsButton->setEnabled(false);
    m_errorDetailsButton->setEnabled(false);
    m_closeButton->setEnabled(true);
    m_closeButton->setDefault(true);

    m_restartRequired = new QLabel(tr("Restart required."), this);
    if (!s_isRestartRequired)
        m_restartRequired->setVisible(false);

    auto hl = new QHBoxLayout;
    hl->addWidget(m_detailsButton);
    hl->addWidget(m_errorDetailsButton);
    hl->addWidget(m_installButton);
    QPushButton *updateButton = new QPushButton(tr("Refresh"));
    const QString tooltip = tr("Scan for newly installed custom widget plugins.");
    updateButton->setToolTip(tooltip);
    updateButton->setWhatsThis(tooltip);
    connect(updateButton, &QAbstractButton::clicked, this, &PluginDialog::updateCustomWidgetPlugins);
    hl->addWidget(updateButton);
    hl->addSpacing(10);
    hl->addWidget(m_restartRequired);
    hl->addStretch(5);
    hl->addWidget(m_closeButton);

    vl->addLayout(hl);

    resize(650, 400);
    setWindowTitle(tr("Installed Plugins"));

    connect(m_view, &ExtensionSystem::PluginView::currentPluginChanged,
            this, &PluginDialog::updateButtons);
    connect(m_view, &ExtensionSystem::PluginView::pluginActivated,
            this, &PluginDialog::openDetails);
    connect(m_view, &ExtensionSystem::PluginView::pluginSettingsChanged,
            this, &PluginDialog::updateRestartRequired);
    connect(m_detailsButton, &QAbstractButton::clicked,
            [this]  { openDetails(m_view->currentPlugin()); });
    connect(m_errorDetailsButton, &QAbstractButton::clicked,
            this, &PluginDialog::openErrorDetails);
    connect(m_installButton, &QAbstractButton::clicked, this, &PluginDialog::showInstallWizard);
    connect(m_closeButton, &QAbstractButton::clicked, this, &PluginDialog::closeDialog);
    updateButtons();

}

void PluginDialog::closeDialog()
{
    ExtensionSystem::PluginManager::writeSettings();
//    if (s_isRestartRequired) {
//        RestartDialog restartDialog(ICore::dialogParent(),
//                                    tr("Plugin changes will take effect after restart."));
//        restartDialog.exec();
//    }//ltc
    accept();
}

void PluginDialog::showInstallWizard()
{
    if (Core::Internal::PluginInstallWizard::exec())
        updateRestartRequired(nullptr);
}

void PluginDialog::updateCustomWidgetPlugins()
{
    m_core->integration()->updateCustomWidgetPlugins();
}

void PluginDialog::updateRestartRequired(ExtensionSystem::PluginSpec *)
{
    // just display the notice all the time after once changing something
//    s_isRestartRequired = true;
//    m_restartRequired->setVisible(true);
    auto wb = m_core->widgetBox();
    wb->setLoadMode(QDesignerWidgetBoxInterface::LoadCustomWidgetsOnly);
    wb->load();
    auto disabledp= m_view->disabledPlugins();
    for (int cat_idx = 0; cat_idx < wb->categoryCount(); ++cat_idx) {
        auto cat = wb->category(cat_idx);
        QList<int> rmidx;
        for (int wid_idx = 0; wid_idx < cat.widgetCount(); wid_idx++) {
            auto wid = cat.widget(wid_idx);
            if (disabledp.contains(wid.name())) {
                rmidx.push_back(wid_idx);
            }
        }
        for(auto iter=rmidx.rbegin();iter!=rmidx.rend();iter++)
        {
            wb->removeWidget(cat_idx,*iter);
        }
    }
}

void PluginDialog::updateButtons()
{
    ExtensionSystem::PluginSpec *selectedSpec = m_view->currentPlugin();
    if (selectedSpec) {
        m_detailsButton->setEnabled(true);
        m_errorDetailsButton->setEnabled(selectedSpec->hasError());
    } else {
        m_detailsButton->setEnabled(false);
        m_errorDetailsButton->setEnabled(false);
    }
}

void PluginDialog::openDetails(ExtensionSystem::PluginSpec *spec)
{
    if (!spec)
        return;

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Plugin Details of %1").arg(spec->name()));
    auto layout = new QVBoxLayout;
    dialog.setLayout(layout);

//    auto details = new ExtensionSystem::PluginDetailsView(&dialog);
//    layout->addWidget(details);
//    details->update(spec);
    QGridLayout *grid=new QGridLayout;
    {
    QHBoxLayout*h1=new QHBoxLayout;
    h1->addStretch();
    h1->addWidget(new QLabel("名称"));
    QHBoxLayout*h2=new QHBoxLayout;
    h2->addWidget(new QLabel(spec->name()));
    h2->addStretch();
    grid->addLayout(h1,0,0);
    grid->addLayout(h2,0,1);
    }
    {
    QHBoxLayout*h1=new QHBoxLayout;
    h1->addStretch();
    h1->addWidget(new QLabel("版本"));
    QHBoxLayout*h2=new QHBoxLayout;
    const QString revision = spec->revision();
    const QString versionString = spec->version() + (revision.isEmpty() ? QString()                                                                       : " (" + revision + ")");
    h2->addWidget(new QLabel(versionString));
    h2->addStretch();
    grid->addLayout(h1,1,0);
    grid->addLayout(h2,1,1);
    }
    {
    QHBoxLayout*h1=new QHBoxLayout;
    h1->addStretch();
    h1->addWidget(new QLabel("位置"));
    QHBoxLayout*h2=new QHBoxLayout;
    h2->addWidget(new QLabel(QDir::toNativeSeparators(spec->filePath())));
    h2->addStretch();
    grid->addLayout(h1,2,0);
    grid->addLayout(h2,2,1);
    }
    layout->addLayout(grid);
    ModelPluginFunction tablemodel;
    QPluginLoader loader(spec->filePath());
    QObject*poInstance;
    QDesignerCustomWidgetInterface*poInterface;
    QWidget*poWidget;
    if(loader.load()
                && (poInstance=loader.instance())
        &&(poInterface=qobject_cast<QDesignerCustomWidgetInterface*>(poInstance))
        &&(poWidget=poInterface->createWidget(nullptr)))
    {
        auto metaObject=poWidget->metaObject();
        for(int i = /*metaObject->methodOffset()*/0; i < metaObject->methodCount(); ++i)
        {
            QString strmethod=QString::fromLatin1(metaObject->method(i).methodSignature());
            TypeofPluginFunction func;
            func.name=strmethod.left(strmethod.indexOf('('));
            func.arg=strmethod.mid(strmethod.indexOf('('));
            func.ret=metaObject->method(i).typeName();
            tablemodel.mdldata.push_back(func);
        }
        delete poWidget;
    }
    QTableView tableview;
    tableview.setModel(&tablemodel);
    layout->addWidget(&tableview);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.resize(400, 800);
    dialog.exec();
}

void PluginDialog::openErrorDetails()
{
    ExtensionSystem::PluginSpec *spec = m_view->currentPlugin();
    if (!spec)
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Plugin Errors of %1").arg(spec->name()));
    auto layout = new QVBoxLayout;
    dialog.setLayout(layout);
    auto errors = new ExtensionSystem::PluginErrorView(&dialog);
    layout->addWidget(errors);
    errors->update(spec);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.resize(500, 300);
    dialog.exec();
}

int ModelPluginFunction::rowCount(const QModelIndex &parent) const
{
    return mdldata.size();
}

int ModelPluginFunction::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant ModelPluginFunction::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return mdldata.at(index.row()).name;
        if (index.column() == 1)
        {
            return mdldata.at(index.row()).arg;
        }
        if (index.column() == 2)
            return mdldata.at(index.row()).ret;
    }
    return QVariant();
}

QVariant ModelPluginFunction::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
            return "函数名";
        if (section == 1)
            return "参数";
        if (section == 2)
            return "返回值";
    }
    return QVariant();
}

} // namespace Internal
} // namespace Core

QT_END_NAMESPACE
