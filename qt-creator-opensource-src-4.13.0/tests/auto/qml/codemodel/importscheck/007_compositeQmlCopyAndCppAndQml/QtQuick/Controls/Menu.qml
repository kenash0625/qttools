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
import QtQuick.Controls.Styles 1.1
import QtQuick.Controls.Private 1.0

/*!
    \qmltype Menu
    \inqmlmodule QtQuick.Controls
    \since 5.1
    \ingroup menus
    \brief Provides a menu component for use in menu bars, as context menu,
    and other popup menus.

    \code
    Menu {
        title: "Edit"

        MenuItem {
            text: "Cut"
            shortcut: "Ctrl+X"
            onTriggered: ...
        }

        MenuItem {
            text: "Copy"
            shortcut: "Ctrl+C"
            onTriggered: ...
        }

        MenuItem {
            text: "Paste"
            shortcut: "Ctrl+V"
            onTriggered: ...
        }

        MenuSeparator { }

        Menu {
            title: "More Stuff"

            MenuItem {
                text: "Do Nothing"
            }
        }
    }
    \endcode

    The main uses for menus:
    \list
    \li
       as a \e top-level menu in a \l MenuBar
    \li
       as a \e submenu inside another menu
    \li
       as a standalone or \e context menu
    \endlist

    Note that some properties, such as \c enabled, \c text, or \c iconSource,
    only make sense in a particular use case of the menu.

    \sa MenuBar, MenuItem, MenuSeparator
*/

MenuPrivate {
    id: root

    /*! \internal
      \omit
      Documented in qqquickmenu.cpp.
      \endomit
    */
    function addMenu(title) {
        return root.insertMenu(items.length, title)
    }

    /*! \internal
      \omit
      Documented in qquickmenu.cpp.
      \endomit
    */
    function insertMenu(index, title) {
        if (!__selfComponent)
            __selfComponent = Qt.createComponent("Menu.qml", root)
        var submenu = __selfComponent.createObject(__selfComponent, { "title": title })
        root.insertItem(index, submenu)
        return submenu
    }

    /*! \internal */
    property Component __selfComponent: null

    /*! \internal */
    property Component style: Qt.createComponent(Settings.style + "/MenuStyle.qml", root)

    /*! \internal */
    property var __parentContentItem: __parentMenu.__contentItem
    /*! \internal */
    property int __currentIndex: -1
    /*! \internal */
    on__MenuClosed: __currentIndex = -1

    /*! \internal */
    __contentItem: Loader {
        sourceComponent: MenuContentItem {
            menu: root
        }
        active: !root.__isNative && root.__popupVisible
        focus: true
        Keys.forwardTo: item ? [item, root.__parentContentItem] : []
        property bool altPressed: root.__parentContentItem ? root.__parentContentItem.altPressed : false
    }
}
