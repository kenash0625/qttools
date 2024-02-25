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
#include <QskPushButton.h>
#include <QskLinearBox.h>
#include <QskWindow.h>
#include <QHBoxLayout>
QT_BEGIN_NAMESPACE

QskPushButton2Plugin::QskPushButton2Plugin(QObject *parent)
    : QObject(parent)
{
}

QString QskPushButton2Plugin::name() const
{
    return QStringLiteral("QskPushButton2");
}

QString QskPushButton2Plugin::group() const
{
    return QStringLiteral("Display Widgets");
}

QString QskPushButton2Plugin::toolTip() const
{
    return QStringLiteral("A widget for displaying a Qt Quick 2 user interface.");
}

QString QskPushButton2Plugin::whatsThis() const
{
    return toolTip();
}

QString QskPushButton2Plugin::includeFile() const
{
    return QStringLiteral("<QtQuickWidgets/QQuickWidget>");
}

QIcon QskPushButton2Plugin::icon() const
{
    return QIcon(QStringLiteral(":/qt-project.org/qquickwidget/images/qquickwidget.png"));
}

bool QskPushButton2Plugin::isContainer() const
{
    return false;
}

QWidget *QskPushButton2Plugin::createWidget(QWidget *parent)
{
//    QQuickWidget *result = new QQuickWidget(parent);
//    connect(result, &QQuickWidget::sceneGraphError,
//            this, &QskPushButton2Plugin::sceneGraphError);
//    return result;
    auto box = new QskLinearBox( Qt::Horizontal );

    /*
        some design systems work with transparencies ( f.e Fluent2 )
        and we need to have a control providing a solid base color
        as bottom layer.
     */
    box->setPanel( true );

    (void) new QskPushButton( "Button 1", box );
    (void) new QskPushButton( "Button 2", box );

    QskWindow *window=new QskWindow;
    window->addItem( box );
    auto w= QWidget::createWindowContainer(window);
    QskPushButton2*r=new QskPushButton2(parent);
    QHBoxLayout*layout=new QHBoxLayout;
    r->setLayout(layout);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    r->setContentsMargins(0,0,0,0);
    layout->addWidget(w);
    return r;
}

bool QskPushButton2Plugin::isInitialized() const
{
    return m_initialized;
}

void QskPushButton2Plugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
    if (m_initialized)
        return;

    m_initialized = true;
}

QString QskPushButton2Plugin::domXml() const
{
    return QStringLiteral("\
    <ui language=\"c++\">\
        <widget class=\"QskPushButton2\" name=\"QskPushButton2\">\
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

void QskPushButton2Plugin::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    qWarning() << Q_FUNC_INFO << ':' << message;
}

QT_END_NAMESPACE

QskPushButton2::QskPushButton2(QWidget *parent):QQuickWidget(parent)
{
}
