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

#include "qquickwidget_plugin.h"

#include <QtDesigner/default_extensionfactory.h>
#include <QtDesigner/qextensionmanager.h>

#include <QtCore/qplugin.h>
#include <QtCore/qdebug.h>
#include <QtQuickWidgets/qquickwidget.h>
#include <QskTextLabel.h>
#include <QskLinearBox.h>
#include <QskQml.h>
#include <QskWindow.h>
#include <QHBoxLayout>
#include <QQuickView>
#include <QPushButton>
#include <QQmlEngine>
#include <QQmlContext>
#include <QskProgressBar.h>
QT_BEGIN_NAMESPACE

QskProgressBarPlugin::QskProgressBarPlugin(QObject *parent)
    : QObject(parent)
{
}

QString QskProgressBarPlugin::name() const
{
    return QStringLiteral("QskProgressBar2");
}

QString QskProgressBarPlugin::group() const
{
    return QStringLiteral("Display Widgets");
}

QString QskProgressBarPlugin::toolTip() const
{
    return QStringLiteral("A widget for displaying a Qt Quick 2 user interface.");
}

QString QskProgressBarPlugin::whatsThis() const
{
    return toolTip();
}

QString QskProgressBarPlugin::includeFile() const
{
    return QStringLiteral("<QtQuickWidgets/QQuickWidget>");
}

QIcon QskProgressBarPlugin::icon() const
{
    return QIcon(QStringLiteral(":/qt-project.org/qquickwidget/images/qquickwidget.png"));
}

bool QskProgressBarPlugin::isContainer() const
{
    return false;
}

QWidget *QskProgressBarPlugin::createWidget(QWidget *parent)
{
    QskQml::registerTypes();
    QskProgressBar2* w=new QskProgressBar2(parent);
    w->engine()->rootContext()->setContextProperty("QskProgressBar2", w);
    w->setSource(QUrl( "qrc:/qt-project.org/qquickwidget/QskProgressBarPlugin.qml" ));
    w->resize(200,200);

    return w;
}

bool QskProgressBarPlugin::isInitialized() const
{
    return m_initialized;
}

void QskProgressBarPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
    if (m_initialized)
        return;

    m_initialized = true;
}

QString QskProgressBarPlugin::domXml() const
{
    return QStringLiteral("\
    <ui language=\"c++\">\
        <widget class=\"QskProgressBar2\" name=\"QskProgressBar2\">\
            <property name=\"geometry\">\
                <rect>\
                    <x>0</x>\
                    <y>0</y>\
                    <width>300</width>\
                    <height>200</height>\
                </rect>\
            </property>\
        </widget>\
    </ui>");
}

void QskProgressBarPlugin::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    qWarning() << Q_FUNC_INFO << ':' << message;
}

QT_END_NAMESPACE

QskProgressBar2::QskProgressBar2(QWidget *parent):QQuickWidget(parent)
{
}
