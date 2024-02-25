/****************************************************************************
**
** Copyright (C) 2020 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms and
** conditions see http://www.qt.io/terms-conditions. For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "tst_blackboxbaremetal.h"

#include "../shared.h"

#include <QtCore/qregularexpression.h>

static bool extractToolset(const QByteArray &output,
                           QByteArray &toolchain, QByteArray &architecture)
{
    const QRegularExpression re("%%([\\w\\-]+)%%, %%(\\w+)%%");
    QRegularExpressionMatchIterator it = re.globalMatch(output);
    if (!it.hasNext())
        return false;
    const QRegularExpressionMatch match = it.next();
    toolchain = match.captured(1).toLocal8Bit();
    architecture = match.captured(2).toLocal8Bit();
    return true;
}

static QByteArray unsupportedToolsetMessage(const QByteArray &output)
{
    QByteArray toolchain;
    QByteArray architecture;
    extractToolset(output, toolchain, architecture);
    return "Unsupported toolchain '" + toolchain
          + "' for architecture '" + architecture + "'";
}

static QString linkerMapFileExtension(const QByteArray &toolchain, const QByteArray &architecture)
{
    if (toolchain == "keil") {
        if (architecture == "mcs51")
            return QStringLiteral(".m51");
        if (architecture == "c166")
            return QStringLiteral(".m66");
    }
    return QStringLiteral(".map");
}

TestBlackboxBareMetal::TestBlackboxBareMetal()
    : TestBlackboxBase (SRCDIR "/testdata-baremetal", "blackbox-baremetal")
{
}

void TestBlackboxBareMetal::targetPlatform()
{
    QDir::setCurrent(testDataDir + "/target-platform");
    QCOMPARE(runQbs(QbsRunParameters("resolve", QStringList("-n"))), 0);
    if (m_qbsStdout.contains("unsupported toolset:"))
        QSKIP(unsupportedToolsetMessage(m_qbsStdout));
    const bool hasNoPlatform = m_qbsStdout.contains("has no platform: true");
    QCOMPARE(hasNoPlatform, true);
    const bool hasNoOS = m_qbsStdout.contains("has no os: true");
    QCOMPARE(hasNoOS, true);
}

void TestBlackboxBareMetal::application_data()
{
    QTest::addColumn<QString>("testPath");
    QTest::newRow("one-object-application") << "/one-object-application";
    QTest::newRow("two-object-application") << "/two-object-application";
    QTest::newRow("one-object-asm-application") << "/one-object-asm-application";
}

void TestBlackboxBareMetal::application()
{
    QFETCH(QString, testPath);
    QDir::setCurrent(testDataDir + testPath);
    QCOMPARE(runQbs(QbsRunParameters("resolve", QStringList("-n"))), 0);
    if (m_qbsStdout.contains("unsupported toolset:"))
        QSKIP(unsupportedToolsetMessage(m_qbsStdout));
    QCOMPARE(runQbs(), 0);
}

void TestBlackboxBareMetal::staticLibraryDependencies()
{
    QDir::setCurrent(testDataDir + "/static-library-dependencies");
    QCOMPARE(runQbs(QStringList{"-p", "lib-a,lib-b,lib-c,lib-d,lib-e"}), 0);
    QCOMPARE(runQbs(QStringList{"--command-echo-mode", "command-line"}), 0);
    const QByteArray output = m_qbsStdout + '\n' + m_qbsStderr;
    QVERIFY(output.contains("lib-a"));
    QVERIFY(output.contains("lib-b"));
    QVERIFY(output.contains("lib-c"));
    QVERIFY(output.contains("lib-d"));
    QVERIFY(output.contains("lib-e"));
}

void TestBlackboxBareMetal::externalStaticLibraries()
{
    QDir::setCurrent(testDataDir + "/external-static-libraries");
    QCOMPARE(runQbs(QbsRunParameters("resolve", QStringList("-n"))), 0);
    if (m_qbsStdout.contains("unsupported toolset:"))
        QSKIP(unsupportedToolsetMessage(m_qbsStdout));
    QCOMPARE(runQbs(), 0);
}

void TestBlackboxBareMetal::userIncludePaths()
{
    QDir::setCurrent(testDataDir + "/user-include-paths");
    QCOMPARE(runQbs(), 0);
}

void TestBlackboxBareMetal::systemIncludePaths()
{
    QDir::setCurrent(testDataDir + "/system-include-paths");
    QCOMPARE(runQbs(), 0);
}

void TestBlackboxBareMetal::distributionIncludePaths()
{
    QDir::setCurrent(testDataDir + "/distribution-include-paths");
    QCOMPARE(runQbs(), 0);
}

void TestBlackboxBareMetal::preincludeHeaders()
{
    QDir::setCurrent(testDataDir + "/preinclude-headers");
    QCOMPARE(runQbs(QbsRunParameters("resolve", QStringList("-n"))), 0);
    if (m_qbsStdout.contains("unsupported toolset:"))
        QSKIP(unsupportedToolsetMessage(m_qbsStdout));
    QCOMPARE(runQbs(), 0);
}

void TestBlackboxBareMetal::defines()
{
    QDir::setCurrent(testDataDir + "/defines");
    QCOMPARE(runQbs(), 0);
}

void TestBlackboxBareMetal::compilerListingFiles_data()
{
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<bool>("generateListing");
    QTest::newRow("do-not-generate-compiler-listing") << "/do-not-generate-compiler-listing" << false;
    QTest::newRow("generate-compiler-listing") << "/generate-compiler-listing" << true;
}

void TestBlackboxBareMetal::compilerListingFiles()
{
    QFETCH(QString, testPath);
    QFETCH(bool, generateListing);
    QDir::setCurrent(testDataDir + testPath);
    QCOMPARE(runQbs(QbsRunParameters("resolve", QStringList("-n"))), 0);
    if (m_qbsStdout.contains("unsupported toolset:"))
        QSKIP(unsupportedToolsetMessage(m_qbsStdout));
    QCOMPARE(runQbs(), 0);
    const bool isShortListingNames = m_qbsStdout.contains("using short listing file names");
    const QString productName = testPath.mid(1);
    const QString productBuildDir = relativeProductBuildDir(productName);
    const QString hash = inputDirHash(".");
    const QString mainListing = productBuildDir + "/" + hash + (isShortListingNames ? "/main.lst" : "/main.c.lst");
    QCOMPARE(regularFileExists(mainListing), generateListing);
    const QString funListing = productBuildDir + "/" + hash + (isShortListingNames ? "/fun.lst" : "/fun.c.lst");
    QCOMPARE(regularFileExists(funListing), generateListing);
}

void TestBlackboxBareMetal::linkerMapFile_data()
{
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<bool>("generateMap");
    QTest::newRow("do-not-generate-linker-map") << "/do-not-generate-linker-map" << false;
    QTest::newRow("generate-linker-map") << "/generate-linker-map" << true;
}

void TestBlackboxBareMetal::linkerMapFile()
{
    QFETCH(QString, testPath);
    QFETCH(bool, generateMap);
    QDir::setCurrent(testDataDir + testPath);
    QCOMPARE(runQbs(QbsRunParameters("resolve", QStringList("-n"))), 0);
    if (m_qbsStdout.contains("unsupported toolset:"))
        QSKIP(unsupportedToolsetMessage(m_qbsStdout));
    if (!m_qbsStdout.contains("current toolset:"))
        QFAIL("No current toolset pattern exists");

    QByteArray toolchain;
    QByteArray architecture;
    if (!extractToolset(m_qbsStdout, toolchain, architecture))
        QFAIL("Unable to extract current toolset");

    QCOMPARE(runQbs(), 0);
    const QString productName = testPath.mid(1);
    const QString productBuildDir = relativeProductBuildDir(productName);
    const auto extension = linkerMapFileExtension(toolchain, architecture);
    const QString linkerMap = productBuildDir + "/" + productName + extension;
    QCOMPARE(regularFileExists(linkerMap), generateMap);
}

QTEST_MAIN(TestBlackboxBareMetal)
