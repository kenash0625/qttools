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
    \qmltype RadioButton
    \inqmlmodule QtQuick.Controls
    \since 5.1
    \ingroup controls
    \brief A radio button with a text label.

    A RadioButton is an option button that can be switched on (checked) or off
    (unchecked). Radio buttons typically present the user with a "one of many"
    choice. In a group of radio buttons, only one radio button at a time can be
    checked; if the user selects another button, the previously selected button
    is switched off.

    \qml
    GroupBox {
        title: qsTr("Search")
        Column {
            ExclusiveGroup { id: group }
            RadioButton {
                text: qsTr("From top")
                exclusiveGroup: group
                checked: true
            }
            RadioButton {
                text: qsTr("From cursor")
                exclusiveGroup: group
            }
        }
    }
    \endqml

    You can create a custom appearance for a RadioButton by
    assigning a \l {QtQuick.Controls.Styles::RadioButtonStyle}{RadioButtonStyle}.
*/

AbstractCheckable {
    id: radioButton

    activeFocusOnTab: true

    Accessible.role: Accessible.RadioButton

    /*!
        The style that should be applied to the radio button. Custom style
        components can be created with:

        \codeline Qt.createComponent("path/to/style.qml", radioButtonId);
    */
    style: Qt.createComponent(Settings.style + "/RadioButtonStyle.qml", radioButton)

    __cycleStatesHandler: function() { checked = !checked; }
}
