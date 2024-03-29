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
    \qmltype SpinBox
    \inqmlmodule QtQuick.Controls
    \since 5.1
    \ingroup controls
    \brief Provides a spin box control.

    SpinBox allows the user to choose a value by clicking the up or down buttons, or by
    pressing up or down on the keyboard. The user can also type the value in manually.

    By default the SpinBox provides discrete values in the range [0-99] with a \l stepSize of 1 and 0 \l decimals.

    \code
    SpinBox {
        id: spinbox
    }
    \endcode

    Note that if you require decimal values you will need to set the \l decimals to a non 0 value.

    \code
    SpinBox {
        id: spinbox
        decimals: 2
    }
    \endcode

*/

Control {
    id: spinbox

    /*!
        \qmlproperty real SpinBox::value

        The value of this SpinBox, clamped to \l minimumValue and \l maximumValue.

        The default value is \c{0.0}.
    */
    property alias value: validator.value

    /*!
        \qmlproperty real SpinBox::minimumValue

        The minimum value of the SpinBox range.
        The \l value is clamped to this value.

        The default value is \c{0.0}.
    */
    property alias minimumValue: validator.minimumValue

    /*!
        \qmlproperty real SpinBox::maximumValue

        The maximum value of the SpinBox range.
        The \l value is clamped to this value. If maximumValue is smaller than
        \l minimumValue, \l minimumValue will be enforced.

        The default value is \c{99}.
    */
    property alias maximumValue: validator.maximumValue

    /*! \qmlproperty real SpinBox::stepSize
        The amount by which the \l value is incremented/decremented when a
        spin button is pressed.

        The default value is \c{1.0}.
    */
    property alias stepSize: validator.stepSize

    /*! \qmlproperty string SpinBox::suffix
        The suffix for the value. I.e "cm" */
    property alias suffix: validator.suffix

    /*! \qmlproperty string SpinBox::prefix
        The prefix for the value. I.e "$" */
    property alias prefix: validator.prefix

    /*! \qmlproperty int SpinBox::decimals
      This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.
      The default value is \c{0}.
    */
    property alias decimals: validator.decimals

    /*! \qmlproperty font SpinBox::font

        This property indicates the current font used by the SpinBox.
    */
    property alias font: input.font

    /*! This property indicates whether the Spinbox should get active
      focus when pressed.
      The default value is \c true.
    */
    property bool activeFocusOnPress: true

    /*! \qmlproperty enumeration horizontalAlignment
        \since 5.2

        This property indicates how the content is horizontally aligned
        within the text field.

        The supported values are:
        \list
        \li Qt.AlignLeft
        \li Qt.AlignHCenter
        \li Qt.AlignRight
        \endlist

      The default value is style dependent.
    */
    property int horizontalAlignment: __panel ? __panel.horizontalAlignment : Qt.AlignLeft

    /*!
        \qmlproperty bool SpinBox::hovered

        This property indicates whether the control is being hovered.
    */
    readonly property alias hovered: mouseArea.containsMouse

    /*!
        \qmlsignal SpinBox::editingFinished()
        \since 5.2

        This signal is emitted when the Return or Enter key is pressed or
        the control loses focus. Note that if there is a validator
        set on the control and enter/return is pressed, this signal will
        only be emitted if the validator returns an acceptable state.
    */
    signal editingFinished()

    style: Qt.createComponent(Settings.style + "/SpinBoxStyle.qml", spinbox)

    /*! \internal */
    function __increment() {
        validator.increment()
        input.selectValue()
    }

    /*! \internal */
    function __decrement() {
        validator.decrement()
        input.selectValue()
    }

    /*! \internal */
    property alias __text: input.text

    __styleData: QtObject {
        readonly property bool upEnabled: value != maximumValue;
        readonly property alias upHovered: mouseUp.containsMouse
        readonly property alias upPressed: mouseUp.pressed

        readonly property bool downEnabled: value != minimumValue;
        readonly property alias downPressed: mouseDown.pressed
        readonly property alias downHovered: mouseDown.containsMouse

        readonly property int contentHeight: Math.max(input.implicitHeight, 16)
        readonly property int contentWidth: Math.max(maxSizeHint.implicitWidth, minSizeHint.implicitWidth)
    }

    Text {
        id: maxSizeHint
        text: prefix + maximumValue.toFixed(decimals) + suffix
        font: input.font
        visible: false
    }

    Text {
        id: minSizeHint
        text: prefix + minimumValue.toFixed(decimals) + suffix
        font: input.font
        visible: false
    }

    activeFocusOnTab: true

    onActiveFocusChanged: if (activeFocus) input.selectValue()

    Accessible.name: input.text
    Accessible.role: Accessible.SpinBox

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed: if (activeFocusOnPress) input.forceActiveFocus()
        onWheel: {
            if (wheel.angleDelta.y > 0)
                __increment();
            else
                __decrement();
        }
    }

    TextInput {
        id: input
        clip: contentWidth > width
        anchors.fill: parent
        anchors.leftMargin: __style ? __style.padding.left : 0
        anchors.topMargin: __style ? __style.padding.top : 0
        anchors.rightMargin: __style ? __style.padding.right: 0
        anchors.bottomMargin: __style ? __style.padding.bottom: 0

        focus: true
        activeFocusOnPress: spinbox.activeFocusOnPress

        horizontalAlignment: spinbox.horizontalAlignment
        verticalAlignment: __panel ? __panel.verticalAlignment : Qt.AlignVCenter
        selectByMouse: true

        validator: SpinBoxValidator {
            id: validator
            property bool ready: false // Delay validation until all properties are ready
            onTextChanged: if (ready) input.text = validator.text
            Component.onCompleted: {input.text = validator.text ; ready = true}
        }
        onAccepted: {
            input.text = validator.text
            selectValue()
        }

        onEditingFinished: spinbox.editingFinished()

        color: __panel ? __panel.foregroundColor : "black"
        selectionColor: __panel ? __panel.selectionColor : "black"
        selectedTextColor: __panel ? __panel.selectedTextColor : "black"

        opacity: parent.enabled ? 1 : 0.5
        renderType: __style ? __style.renderType : Text.NativeRendering

        function selectValue() {
            select(prefix.length, text.length - suffix.length)
        }
    }

    // Spinbox increment button

    MouseArea {
        id: mouseUp
        objectName: "mouseUp"
        hoverEnabled: true

        property var upRect: __panel  ?  __panel.upRect : null

        anchors.left: parent.left
        anchors.top: parent.top

        anchors.leftMargin: upRect ? upRect.x : 0
        anchors.topMargin: upRect ? upRect.y : 0

        width: upRect ? upRect.width : 0
        height: upRect ? upRect.height : 0

        onClicked: __increment()
        onPressed: if (activeFocusOnPress) input.forceActiveFocus()

        property bool autoincrement: false;
        onReleased: autoincrement = false
        Timer { running: mouseUp.pressed; interval: 350 ; onTriggered: mouseUp.autoincrement = true }
        Timer { running: mouseUp.autoincrement; interval: 60 ; repeat: true ; onTriggered: __increment() }
    }

    // Spinbox decrement button

    MouseArea {
        id: mouseDown
        objectName: "mouseDown"
        hoverEnabled: true

        onClicked: __decrement()
        onPressed: if (activeFocusOnPress) input.forceActiveFocus()

        property var downRect: __panel ? __panel.downRect : null

        anchors.left: parent.left
        anchors.top: parent.top

        anchors.leftMargin: downRect ? downRect.x : 0
        anchors.topMargin: downRect ? downRect.y : 0

        width: downRect ? downRect.width : 0
        height: downRect ? downRect.height : 0

        property bool autoincrement: false;
        onReleased: autoincrement = false
        Timer { running: mouseDown.pressed; interval: 350 ; onTriggered: mouseDown.autoincrement = true }
        Timer { running: mouseDown.autoincrement; interval: 60 ; repeat: true ; onTriggered: __decrement() }
    }

    Keys.onUpPressed: __increment()
    Keys.onDownPressed: __decrement()
}
