/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include <QtGlobal>

#if defined(LANGUAGECLIENT_LIBRARY)
#  define LANGUAGECLIENT_EXPORT Q_DECL_EXPORT
#else
#  define LANGUAGECLIENT_EXPORT Q_DECL_IMPORT
#endif

namespace LanguageClient {
namespace Constants {

const char LANGUAGECLIENT_SETTINGS_CATEGORY[] = "ZY.LanguageClient";
const char LANGUAGECLIENT_SETTINGS_PAGE[] = "LanguageClient.General";
const char LANGUAGECLIENT_SETTINGS_TR[] = QT_TRANSLATE_NOOP("LanguageClient", "Language Client");
const char LANGUAGECLIENT_DOCUMENT_FILTER_ID[] = "Current Document Symbols";
const char LANGUAGECLIENT_DOCUMENT_FILTER_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("LanguageClient", "Symbols in Current Document");
const char LANGUAGECLIENT_WORKSPACE_FILTER_ID[] = "Workspace Symbols";
const char LANGUAGECLIENT_WORKSPACE_FILTER_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("LanguageClient", "Symbols in Workspace");
const char LANGUAGECLIENT_WORKSPACE_CLASS_FILTER_ID[] = "Workspace Classes and Structs";
const char LANGUAGECLIENT_WORKSPACE_CLASS_FILTER_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("LanguageClient", "Classes and Structs in Workspace");
const char LANGUAGECLIENT_WORKSPACE_METHOD_FILTER_ID[] = "Workspace Functions and Methods";
const char LANGUAGECLIENT_WORKSPACE_METHOD_FILTER_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("LanguageClient", "Functions and Methods in Workspace");

} // namespace Constants
} // namespace LanguageClient
