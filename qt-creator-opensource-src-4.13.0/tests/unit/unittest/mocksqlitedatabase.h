/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include "googletest.h"

#include "mocksqlitereadstatement.h"
#include "mocksqlitetransactionbackend.h"
#include "mocksqlitewritestatement.h"

#include <sqlitedatabaseinterface.h>
#include <sqlitetable.h>
#include <sqlitetransaction.h>

#include <utils/smallstringview.h>

class MockSqliteDatabase : public MockSqliteTransactionBackend, public Sqlite::DatabaseInterface
{
public:
    using ReadStatement = NiceMock<MockSqliteReadStatement>;
    using WriteStatement = NiceMock<MockSqliteWriteStatement>;

    MOCK_METHOD1(execute,
                 void (Utils::SmallStringView sqlStatement));

    MOCK_CONST_METHOD0(lastInsertedRowId,
                       int64_t ());

    MOCK_CONST_METHOD1(setLastInsertedRowId,
                       void (int64_t));

    MOCK_CONST_METHOD0(isInitialized,
                      bool ());

    MOCK_METHOD1(setIsInitialized,
                 void (bool));

    MOCK_METHOD0(walCheckpointFull, void());

    MOCK_METHOD2(setUpdateHook,
                 void(void *object,
                      void (*)(void *object, int, char const *database, char const *, long long rowId)));

    MOCK_METHOD0(resetUpdateHook, void());

    MOCK_METHOD0(applyAndUpdateSessions, void());

    MOCK_METHOD1(setAttachedTables, void(const Utils::SmallStringVector &tables));
};

