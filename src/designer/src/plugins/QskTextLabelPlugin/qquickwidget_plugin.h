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

#ifndef QQUICKWIDGET_PLUGIN_H
#define QQUICKWIDGET_PLUGIN_H

#include <QtQuick/qquickwindow.h>
#include <QtUiPlugin/customwidget.h>
#include <QQuickWidget>
QT_BEGIN_NAMESPACE
class Singleton : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString thing READ thing WRITE setThing NOTIFY thingChanged FINAL)
    QML_ELEMENT
    QML_SINGLETON

public:
    Singleton(QObject *parent = nullptr) : QObject(parent) {}

    QString thing() const { return m_value; }
    void setThing(QString v)
    {
        if (v != m_value) {
            m_value = v;
            emit thingChanged();
        }
    }

signals:
    void thingChanged();

private:
    QString m_value = "12";
};

class QskTextLabelPlugin: public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit QskTextLabelPlugin(QObject *parent = nullptr);

    QString name() const override;
    QString group() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QString includeFile() const override;
    QIcon icon() const override;
    bool isContainer() const override;
    QWidget *createWidget(QWidget *parent) override;
    bool isInitialized() const override;
    void initialize(QDesignerFormEditorInterface *core) override;
    QString domXml() const override;

private slots:
    void sceneGraphError(QQuickWindow::SceneGraphError, const QString &);

private:
    bool m_initialized = false;
    QScopedPointer<Singleton> example;
};
class QskTextLabel2:public QQuickWidget
{
    Q_OBJECT
    Q_PROPERTY(QString thing READ thing WRITE setThing NOTIFY thingChanged FINAL)
    Q_PROPERTY(int width READ width WRITE setwidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height READ height WRITE setheight NOTIFY heightChanged FINAL)

public:
    QskTextLabel2(QWidget*parent);

    QString thing() const { return m_value; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    void setwidth(int v)
    {
        if (v != m_width) {
            m_width = v;
            emit widthChanged();
        }
    }
    void setheight(int v)
    {
        if (v != m_height) {
            m_height = v;
            emit heightChanged();
        }
    }
    void setThing(QString v)
    {
        if (v != m_value) {
            m_value = v;
            emit thingChanged();
        }
    }

signals:
    void thingChanged();
    void widthChanged();
    void heightChanged();

private:
    QString m_value = "12";
    int m_width=200,m_height=200;
};
QT_END_NAMESPACE

#endif // QQUICKWIDGET_PLUGIN_H
