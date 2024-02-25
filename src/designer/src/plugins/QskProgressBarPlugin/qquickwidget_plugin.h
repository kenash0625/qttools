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

class QskProgressBarPlugin: public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit QskProgressBarPlugin(QObject *parent = nullptr);

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
};
class QskProgressBar2:public QQuickWidget
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setwidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height READ height WRITE setheight NOTIFY heightChanged FINAL)
    Q_PROPERTY(double minvalue READ minvalue WRITE setminvalue NOTIFY minvalueChanged FINAL)
    Q_PROPERTY(double maxvalue READ maxvalue WRITE setmaxvalue NOTIFY maxvalueChanged FINAL)
    Q_PROPERTY( Qt::Orientation orientation READ orientation
                       WRITE setOrientation NOTIFY orientationChanged )

public:
    QskProgressBar2(QWidget*parent);
    Qt::Orientation orientation() const
    {
        return m_orientation;
    }
    void setOrientation( Qt::Orientation orientation )
    {
        if (orientation != m_orientation) {
            m_orientation = orientation;
            emit orientationChanged(m_orientation);
        }
    }

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

    double minvalue() const { return m_minvalue; }
    double maxvalue() const { return m_maxvalue; }
    void setminvalue(double v)
    {
        if (v != m_minvalue) {
            m_minvalue = v;
            emit minvalueChanged();
        }
    }
    void setmaxvalue(double v)
    {
        if (v != m_maxvalue) {
            m_maxvalue = v;
            emit maxvalueChanged();
        }
    }

signals:
    void widthChanged();
    void heightChanged();
    void minvalueChanged();
    void maxvalueChanged();
    void orientationChanged( Qt::Orientation );

private:
    int m_width=200,m_height=200;
    double m_minvalue=0,m_maxvalue=100;
    Qt::Orientation m_orientation=Qt::Horizontal;
};
QT_END_NAMESPACE

#endif // QQUICKWIDGET_PLUGIN_H
