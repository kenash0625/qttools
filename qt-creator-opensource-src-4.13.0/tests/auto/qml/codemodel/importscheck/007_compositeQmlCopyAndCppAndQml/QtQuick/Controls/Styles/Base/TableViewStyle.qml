/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Private 1.0

/*!
    \qmltype TableViewStyle
    \inqmlmodule QtQuick.Controls.Styles
    \since 5.1
    \ingroup viewsstyling
    \brief Provides custom styling for TableView

    Note that this class derives from \l ScrollViewStyle
    and supports all of the properties defined there.
*/
ScrollViewStyle {
    id: root

    /*! The \l TableView attached to this style. */
    readonly property TableView control: __control

    /*! The text color. */
    property color textColor: __syspal.text

    /*! The background color. */
    property color backgroundColor: control.backgroundVisible ? __syspal.base : "transparent"

    /*! The alternate background color. */
    property color alternateBackgroundColor: "#f5f5f5"

    /*! The text highlight color, used within selections. */
    property color highlightedTextColor: "white"

    /*! Activates items on single click. */
    property bool activateItemOnSingleClick: false

    padding.top: control.headerVisible ? 0 : 1

    /*! \qmlproperty Component TableViewStyle::headerDelegate
    Delegate for header. This delegate is described in \l {TableView::headerDelegate}
    */
    property Component headerDelegate: BorderImage {
        height: textItem.implicitHeight * 1.2
        source: "images/header.png"
        border.left: 4
        border.bottom: 2
        border.top: 2
        Text {
            id: textItem
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            anchors.leftMargin: 12
            text: styleData.value
            elide: Text.ElideRight
            color: textColor
            renderType: Text.NativeRendering
        }
        Rectangle {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 1
            anchors.topMargin: 1
            width: 1
            color: "#ccc"
        }
    }

    /*! \qmlproperty Component TableViewStyle::rowDelegate
    Delegate for header. This delegate is described in \l {TableView::rowDelegate}
    */
    property Component rowDelegate: Rectangle {
        height: Math.round(TextSingleton.implicitHeight * 1.2)
        property color selectedColor: styleData.hasActiveFocus ? "#07c" : "#999"
        color: styleData.selected ? selectedColor :
                                    !styleData.alternate ? alternateBackgroundColor : backgroundColor
    }

    /*! \qmlproperty Component TableViewStyle::itemDelegate
    Delegate for item. This delegate is described in \l {TableView::itemDelegate}
    */
    property Component itemDelegate: Item {
        height: Math.max(16, label.implicitHeight)
        property int implicitWidth: label.implicitWidth + 20

        Text {
            id: label
            objectName: "label"
            width: parent.width
            anchors.leftMargin: 12
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: styleData.textAlignment
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 1
            elide: styleData.elideMode
            text: styleData.value !== undefined ? styleData.value : ""
            color: styleData.textColor
            renderType: Text.NativeRendering
        }
    }
}

