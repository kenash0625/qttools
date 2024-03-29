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

#include <iosfwd>

#include <gtest/gtest-printers.h>

namespace llvm {

class StringRef;

std::ostream &operator<<(std::ostream &out, const StringRef stringReference);
}

namespace clang {
class FullSourceLoc;
class SourceRange;
class SourceManager;

void PrintTo(const FullSourceLoc &sourceLocation, ::std::ostream *os);
void PrintTo(const SourceRange &sourceLocation, ::std::ostream *os);

}

namespace TestGlobal {
void setSourceManager(const clang::SourceManager *sourceManager);
}

namespace ClangBackEnd {
class TokenInfo;
template<typename T>
class TokenProcessor;
class SuspendResumeJobsEntry;
class ReferencesResult;

std::ostream &operator<<(std::ostream &os, const TokenInfo &tokenInfo);
template<class T>
std::ostream &operator<<(std::ostream &out, const TokenProcessor<T> &tokenInfos);
std::ostream &operator<<(std::ostream &out, const SuspendResumeJobsEntry &entry);
std::ostream &operator<<(std::ostream &os, const ReferencesResult &value);

} // namespace ClangBackEnd
