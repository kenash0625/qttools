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

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Private 1.0

/*!
    \qmltype SwitchStyle
    \inqmlmodule QtQuick.Controls.Styles
    \since 5.2
    \ingroup controlsstyling
    \brief Provides custom styling for Switch

    Example:
    \qml
    Switch {
        style: SwitchStyle {
            groove: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 20
                    radius: 9
                    border.color: control.activeFocus ? "darkblue" : "gray"
                    border.width: 1
            }
        }
     }
    \endqml
*/
Style {
    id: switchstyle

    /*! The content padding. */
    padding {
        top: 0
        left: 0
        right: 0
        bottom: 0
    }

    /*! This defines the switch handle.  */
    property Component handle: Rectangle {
        opacity: control.enabled ? 1.0 : 0.5
        implicitWidth: Math.round((parent.parent.width - padding.left - padding.right)/2)
        implicitHeight: control.height - padding.top - padding.bottom

        border.color: control.activeFocus ? Qt.darker(__syspal.highlight, 2) : Qt.darker(__syspal.button, 2)
        property color bg: control.activeFocus ? Qt.darker(__syspal.highlight, 1.2) : __syspal.button
        gradient: Gradient {
            GradientStop {color: Qt.lighter(bg, 1.4) ; position: 0}
            GradientStop {color: bg ; position: 1}
        }

        radius: 2
    }

    /*! This property holds the background groove of the switch. */
    property Component groove: Rectangle {
        property color shadow: control.checked ? Qt.darker(__syspal.highlight, 1.2): "#999"
        property color bg: control.checked ? __syspal.highlight:"#bbb"

        implicitWidth: Math.round(implicitHeight * 3)
        implicitHeight: Math.max(16, Math.round(TextSingleton.implicitHeight))

        border.color: "gray"
        color: "red"

        radius: 2
        Behavior on shadow {ColorAnimation{ duration: 80 }}
        Behavior on bg {ColorAnimation{ duration: 80 }}
        gradient: Gradient {
            GradientStop {color: shadow; position: 0}
            GradientStop {color: bg ; position: 0.2}
            GradientStop {color: bg ; position: 1}
        }
        Rectangle {
            color: "#44ffffff"
            height: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -1
            width: parent.width - 2
            x: 1
        }
    }

    /*! \internal */
    property Component panel: Item {

        implicitWidth: Math.round(grooveLoader.width + padding.left + padding.right)
        implicitHeight: grooveLoader.implicitHeight + padding.top + padding.bottom

        property var __groove: grooveLoader
        property var __handle: handleLoader
        property bool enableAnimation: false

        Loader {
            id: grooveLoader
            y: padding.top
            x: padding.left

            sourceComponent: groove
            anchors.verticalCenter: parent.verticalCenter


            Loader {
                id: handleLoader

                z:1
                anchors.top: grooveLoader.top
                anchors.bottom: grooveLoader.bottom
                anchors.topMargin: padding.top
                anchors.bottomMargin: padding.bottom

                Behavior on x {
                    id: behavior
                    enabled: enableAnimation
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.OutCubic
                    }
                }

                sourceComponent: handle
            }
        }
    }
}
