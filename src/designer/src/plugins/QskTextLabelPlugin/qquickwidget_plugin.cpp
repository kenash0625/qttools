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
QT_BEGIN_NAMESPACE

QskTextLabelPlugin::QskTextLabelPlugin(QObject *parent)
    : QObject(parent),example(new Singleton)
{
}

QString QskTextLabelPlugin::name() const
{
    return QStringLiteral("QskTextLabel2");
}

QString QskTextLabelPlugin::group() const
{
    return QStringLiteral("Display Widgets");
}

QString QskTextLabelPlugin::toolTip() const
{
    return QStringLiteral("A widget for displaying a Qt Quick 2 user interface.");
}

QString QskTextLabelPlugin::whatsThis() const
{
    return toolTip();
}

QString QskTextLabelPlugin::includeFile() const
{
    return QStringLiteral("<QtQuickWidgets/QQuickWidget>");
}

QIcon QskTextLabelPlugin::icon() const
{
    return QIcon(QStringLiteral(":/qt-project.org/qquickwidget/images/qquickwidget.png"));
}

bool QskTextLabelPlugin::isContainer() const
{
    return false;
}

QWidget *QskTextLabelPlugin::createWidget(QWidget *parent)
{
    QskQml::registerTypes();
    QskTextLabel2* w=new QskTextLabel2(parent);
    w->engine()->rootContext()->setContextProperty("QskTextLabel2", w);

    w->setSource(QUrl( "qrc:/qt-project.org/qquickwidget/textlabel.qml" ));
    w->resize(200,200);

    return w;
}

bool QskTextLabelPlugin::isInitialized() const
{
    return m_initialized;
}

void QskTextLabelPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
    if (m_initialized)
        return;

    m_initialized = true;
}

QString QskTextLabelPlugin::domXml() const
{
    return QStringLiteral("\
    <ui language=\"c++\">\
        <widget class=\"QskTextLabel2\" name=\"QskTextLabel2\">\
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

void QskTextLabelPlugin::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    qWarning() << Q_FUNC_INFO << ':' << message;
}

QT_END_NAMESPACE

QskTextLabel2::QskTextLabel2(QWidget *parent):QQuickWidget(parent)
{
}
