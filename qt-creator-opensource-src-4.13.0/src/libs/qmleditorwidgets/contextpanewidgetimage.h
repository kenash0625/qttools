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

#include "qmleditorwidgets_global.h"
#include "contextpanewidget.h"
#include <qdrawutil.h>

#include <QLabel>
#include <QPointer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class ContextPaneWidgetImage;
    class ContextPaneWidgetBorderImage;
}
class QLabel;
class QSlider;
QT_END_NAMESPACE

namespace QmlJS { class PropertyReader; }

namespace QmlEditorWidgets {

class FileWidget;

class PreviewLabel : public QLabel
{
    Q_OBJECT

public:
    PreviewLabel(QWidget *parent = nullptr);
    void setZoom(int);
    void setIsBorderImage(bool b);
    void setMargins(int left, int top, int right, int bottom);
    int leftMarging() const { return m_left; }
    int topMarging() const { return m_top; }
    int rightMarging() const { return m_right; }
    int bottomMarging() const { return m_bottom; }

signals:
    void leftMarginChanged();
    void topMarginChanged();
    void bottomMarginChanged();
    void rightMarginChanged();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void leaveEvent(QEvent* event );
private:
    bool m_showBorders;
    int m_left, m_right, m_top, m_bottom;
    bool m_dragging_left;
    bool m_dragging_right;
    bool m_dragging_top;
    bool m_dragging_bottom;
    QPoint m_startPos;
    int m_zoom;
    bool m_borderImage;
    QLabel *m_hooverInfo;
};

class PreviewDialog : public DragWidget
{
    Q_OBJECT

public:
    PreviewDialog(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &p, int zoom = 1);
    void setZoom(int z);
    void setIsBorderImage(bool b);
    PreviewLabel *previewLabel() const;
    int zoom() { return m_zoom; }

    void onTogglePane();
    void onSliderMoved(int value);

protected:
    void wheelEvent(QWheelEvent* event);

private:
    PreviewLabel *m_label;
    QSlider *m_slider;
    QLabel *m_zoomLabel;
    int m_zoom;
    QPixmap m_pixmap;
    bool m_borderImage;
};

class QMLEDITORWIDGETS_EXPORT ContextPaneWidgetImage : public QWidget
{
    Q_OBJECT

public:
    explicit ContextPaneWidgetImage(QWidget *parent = nullptr, bool borderImage = false);
    ~ContextPaneWidgetImage();
    void setProperties(QmlJS::PropertyReader *propertyReader);
    void setPath(const QString& path);
    PreviewDialog* previewDialog();

signals:
    void propertyChanged(const QString &, const QVariant &);
    void removeProperty(const QString &);
    void removeAndChangeProperty(const QString &, const QString &, const QVariant &, bool removeFirst);

public:
    void onStretchChanged();
    void onVerticalStretchChanged();
    void onHorizontalStretchChanged();
    void onFileNameChanged();
    void onPixmapDoubleClicked();
    void setPixmap(const QString &fileName);
    void onLeftMarginsChanged();
    void onTopMarginsChanged();
    void onBottomMarginsChanged();
    void onRightMarginsChanged();

protected:
    void changeEvent(QEvent *e);
    void hideEvent(QHideEvent* event);
    void showEvent(QShowEvent* event);

private:
    Ui::ContextPaneWidgetImage *ui;
    Ui::ContextPaneWidgetBorderImage *uiBorderImage;
    QString m_path;
    QPointer<PreviewDialog> m_previewDialog;
    FileWidget *m_fileWidget;
    QLabel *m_sizeLabel;
    bool m_borderImage;
    bool previewWasVisible;
};

class LabelFilter: public QObject {

    Q_OBJECT
public:
    LabelFilter(QObject* parent =nullptr) : QObject(parent) {}
signals:
    void doubleClicked();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class WheelFilter: public QObject {

    Q_OBJECT
public:
    WheelFilter(QObject* parent =nullptr) : QObject(parent) {}
    void setTarget(QObject *target) { m_target = target; }
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    QObject *m_target;
};





} //QmlDesigner
