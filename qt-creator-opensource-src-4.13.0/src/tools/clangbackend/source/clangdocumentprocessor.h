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

#include "clangjobrequest.h"
#include "clangjobs.h"

#include <memory>

namespace ClangBackEnd {

class ClangCodeModelClientInterface;
class Document;
class Documents;
class DocumentProcessorData;
class JobRequest;
class UnsavedFiles;

class DocumentProcessor
{
public:
    DocumentProcessor(const Document &document,
                      Documents &documents,
                      UnsavedFiles &unsavedFiles,
                      ClangCodeModelClientInterface &client);

    Jobs &jobs() const;

    JobRequest createJobRequest(JobRequest::Type type,
                                PreferredTranslationUnit preferredTranslationUnit
                                        = PreferredTranslationUnit::RecentlyParsed) const;

    void addJob(const JobRequest &jobRequest);
    void addJob(JobRequest::Type type,
                PreferredTranslationUnit preferredTranslationUnit
                        = PreferredTranslationUnit::RecentlyParsed);

    JobRequests process();
    JobRequests stop();

    Document document() const;

    bool hasSupportiveTranslationUnit() const;
    void startInitializingSupportiveTranslationUnit();

public: // for tests
    bool isSupportiveTranslationUnitInitialized() const;
    JobRequests &queue();
    QList<Jobs::RunningJob> runningJobs() const;
    int queueSize() const;

private:
    std::shared_ptr<DocumentProcessorData> d;
};

} // namespace ClangBackEnd
