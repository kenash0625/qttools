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

#include "subcomponentmanager.h"

#include <qmldesignerconstants.h>

#include "model.h"
#include "metainforeader.h"

#include <utils/algorithm.h>
#include <utils/hostosinfo.h>
#include <coreplugin/messagebox.h>

#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
#include <QUrl>

#include <qmljs/qmljslink.h>
#include <qmljs/parser/qmljsast_p.h>
#include <qmljs/parser/qmljsengine_p.h>
#include <qmljs/qmljsmodelmanagerinterface.h>

enum { debug = false };

QT_BEGIN_NAMESPACE

// Allow usage of QFileInfo in Utils::sort

static bool operator<(const QFileInfo &file1, const QFileInfo &file2)
{
    return file1.filePath() < file2.filePath();
}

QT_END_NAMESPACE

namespace QmlDesigner {
static const QString s_qmlFilePattern = QStringLiteral("*.qml");

SubComponentManager::SubComponentManager(Model *model, QObject *parent)
    : QObject(parent),
      m_model(model)
{
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, [this](const QString &path) { parseDirectory(path); });
}

void SubComponentManager::addImport(int pos, const Import &import)
{
    if (debug)
        qDebug() << Q_FUNC_INFO << pos << import.file().toUtf8();

    if (import.isFileImport()) {
        QFileInfo dirInfo = QFileInfo(m_filePath.resolved(import.file()).toLocalFile());
        if (dirInfo.exists() && dirInfo.isDir()) {
            const QString canonicalDirPath = dirInfo.canonicalFilePath();
            m_watcher.addPath(canonicalDirPath);
            //m_dirToQualifier.insertMulti(canonicalDirPath, import.qualifier()); ### todo: proper support for import as
        }
    } else {
        QString url = import.url();

        url.replace(QLatin1Char('.'), QLatin1Char('/'));

        foreach (const QString &path, importPaths()) {
            QFileInfo dirInfo = QFileInfo(path + QLatin1Char('/') + url);
            if (dirInfo.exists() && dirInfo.isDir()) {
                const QString canonicalDirPath = dirInfo.canonicalFilePath();
                m_watcher.addPath(canonicalDirPath);
                //m_dirToQualifier.insertMulti(canonicalDirPath, import.qualifier()); ### todo: proper support for import as
            }
        }
        // TODO: QDeclarativeDomImport::Library
    }

    m_imports.insert(pos, import);
}

void SubComponentManager::removeImport(int pos)
{
    const Import import = m_imports.takeAt(pos);

    if (import.isFileImport()) {
        const QFileInfo dirInfo = QFileInfo(m_filePath.resolved(import.file()).toLocalFile());
        const QString canonicalDirPath = dirInfo.canonicalFilePath();

        //m_dirToQualifier.remove(canonicalDirPath, import.qualifier()); ### todo: proper support for import as

        if (!m_dirToQualifier.contains(canonicalDirPath))
            m_watcher.removePath(canonicalDirPath);

//        foreach (const QFileInfo &monitoredFile, watchedFiles(canonicalDirPath)) { ### todo: proper support for import as
//            if (!m_dirToQualifier.contains(canonicalDirPath))
//                unregisterQmlFile(monitoredFile, import.qualifier());
//        }
    } else {
            // TODO: QDeclarativeDomImport::Library
    }
}

void SubComponentManager::parseDirectories()
{
    if (!m_filePath.isEmpty()) {
        const QString file = m_filePath.toLocalFile();
        QFileInfo dirInfo = QFileInfo(QFileInfo(file).path());
        const QString canonicalPath = dirInfo.canonicalFilePath();
        if (dirInfo.exists() && dirInfo.isDir())
            parseDirectory(canonicalPath);

        foreach (const QString &subDir, QDir(QFileInfo(file).path()).entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot)) {
            parseDirectory(canonicalPath + QLatin1Char('/') + subDir, true, subDir.toUtf8());
        }
    }

    const QStringList assetPaths = quick3DAssetPaths();
    for (const auto &assetPath : assetPaths)
        parseDirectory(assetPath);

    foreach (const Import &import, m_imports) {
        if (import.isFileImport()) {
            QFileInfo dirInfo = QFileInfo(m_filePath.resolved(import.file()).toLocalFile());
            if (dirInfo.exists() && dirInfo.isDir())
                parseDirectory(dirInfo.canonicalFilePath(), true, dirInfo.baseName().toUtf8());
        } else {
            QString url = import.url();
            url.replace(QLatin1Char('.'), QLatin1Char('/'));
            QFileInfo dirInfo = QFileInfo(url);
            foreach (const QString &path, importPaths()) {
                QString fullUrl  = path + QLatin1Char('/') + url;
                dirInfo = QFileInfo(fullUrl);

                if (dirInfo.exists() && dirInfo.isDir()) {
                    //### todo full qualified names QString nameSpace = import.uri();
                    parseDirectory(dirInfo.canonicalFilePath(), false);
                }

                QString fullUrlVersion = path + QLatin1Char('/') + url + QLatin1Char('.') + import.version().split('.').constFirst();
                dirInfo = QFileInfo(fullUrlVersion);

                if (dirInfo.exists() && dirInfo.isDir()) {
                    //### todo full qualified names QString nameSpace = import.uri();
                    parseDirectory(dirInfo.canonicalFilePath(), false);
                }
            }
        }
    }
}

void SubComponentManager::parseDirectory(const QString &canonicalDirPath, bool addToLibrary, const TypeName &qualification)
{
    if (!model() || !model()->rewriterView())
        return;

    if (canonicalDirPath.endsWith(QLatin1String(Constants::QUICK_3D_ASSETS_FOLDER))) {
        parseQuick3DAssetDir(canonicalDirPath);
        return;
    }

    QDir designerDir(canonicalDirPath + QLatin1String(Constants::QML_DESIGNER_SUBFOLDER));
    if (designerDir.exists()) {
        QStringList filter;
        filter << QLatin1String("*.metainfo");
        designerDir.setNameFilters(filter);

        QStringList metaFiles = designerDir.entryList(QDir::Files);
        foreach (const QFileInfo &metaInfoFile, designerDir.entryInfoList(QDir::Files)) {
            if (model() && model()->metaInfo().itemLibraryInfo()) {
                Internal::MetaInfoReader reader(model()->metaInfo());
                reader.setQualifcation(qualification);
                try {
                    reader.readMetaInfoFile(metaInfoFile.absoluteFilePath(), true);
                } catch (const InvalidMetaInfoException &e) {
                    qWarning() << e.description();
                    const QString errorMessage = metaInfoFile.absoluteFilePath() + QLatin1Char('\n') + QLatin1Char('\n') + reader.errors().join(QLatin1Char('\n'));
                    Core::AsynchronousMessageBox::warning(QCoreApplication::translate("SubComponentManager::parseDirectory", "Invalid meta info"),
                                                           errorMessage);
                }
            }
        }
        if (!metaFiles.isEmpty())
            return;
    }

    if (debug)
        qDebug() << Q_FUNC_INFO << canonicalDirPath;

    QDir dir(canonicalDirPath);

    dir.setNameFilters(QStringList(s_qmlFilePattern));
    dir.setFilter(QDir::Files | QDir::Readable | QDir::CaseSensitive);

    QFileInfoList monitoredList = watchedFiles(canonicalDirPath);
    QFileInfoList newList;
    foreach (const QFileInfo &qmlFile, dir.entryInfoList()) {
        if (QFileInfo(m_filePath.toLocalFile()) == qmlFile) {
            // do not parse main file
            continue;
        }
        if (!qmlFile.fileName().at(0).isUpper()) {
            // QML sub components must be upper case
            continue;
        }
        newList << qmlFile;
    }

    Utils::sort(monitoredList);
    Utils::sort(newList);

    if (debug)
        qDebug() << "monitored list " << monitoredList.size() << "new list " << newList.size();
    auto oldIter = monitoredList.constBegin();
    auto newIter = newList.constBegin();

    while (oldIter != monitoredList.constEnd() && newIter != newList.constEnd()) {
        const QFileInfo oldFileInfo = *oldIter;
        const QFileInfo newFileInfo = *newIter;
        if (oldFileInfo == newFileInfo) {
            ++oldIter;
            ++newIter;
            continue;
        }
        if (oldFileInfo < newFileInfo) {
            foreach (const QString &qualifier, m_dirToQualifier.value(canonicalDirPath))
                unregisterQmlFile(oldFileInfo, qualifier);
            m_watcher.removePath(oldFileInfo.filePath());
            ++oldIter;
            continue;
        }
        // oldFileInfo > newFileInfo
        parseFile(newFileInfo.filePath(), addToLibrary, QString::fromUtf8(qualification));
        ++newIter;
    }

    while (oldIter != monitoredList.constEnd()) {
        foreach (const QString &qualifier, m_dirToQualifier.value(canonicalDirPath))
            unregisterQmlFile(*oldIter, qualifier);
        ++oldIter;
    }

    while (newIter != newList.constEnd()) {
        parseFile(newIter->filePath(), addToLibrary, QString::fromUtf8(qualification));
        if (debug)
            qDebug() << "m_watcher.addPath(" << newIter->filePath() << ')';
        ++newIter;
    }
}

void SubComponentManager::parseFile(const QString &canonicalFilePath, bool addToLibrary, const QString &qualification)
{
    if (debug)
        qDebug() << Q_FUNC_INFO << canonicalFilePath;

    QFile file(canonicalFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString dir = QFileInfo(canonicalFilePath).path();
    foreach (const QString &qualifier, m_dirToQualifier.values(dir)) {
        registerQmlFile(canonicalFilePath, qualifier, addToLibrary);
    }
    registerQmlFile(canonicalFilePath, qualification, addToLibrary);
}

void SubComponentManager::parseFile(const QString &canonicalFilePath)
{
    parseFile(canonicalFilePath, true, QString());
}

// dirInfo must already contain a canonical path
QFileInfoList SubComponentManager::watchedFiles(const QString &canonicalDirPath)
{
    QFileInfoList files;

    foreach (const QString &monitoredFile, m_watcher.files()) {
        QFileInfo fileInfo(monitoredFile);
        if (fileInfo.dir().absolutePath() == canonicalDirPath)
            files.append(fileInfo);
    }
    return files;
}

void SubComponentManager::unregisterQmlFile(const QFileInfo &fileInfo, const QString &qualifier)
{
    QString componentName = fileInfo.baseName();
    if (!qualifier.isEmpty())
        componentName = qualifier + QLatin1Char('.') + componentName;
}


void SubComponentManager::registerQmlFile(const QFileInfo &fileInfo, const QString &qualifier,
                                          bool addToLibrary)
{
    if (!model())
        return;

    QString componentName = fileInfo.baseName();
    const QString baseComponentName = componentName;

    QString fixedQualifier = qualifier;
    if (!qualifier.isEmpty()) {
        fixedQualifier = qualifier;
        if (qualifier.right(1) == QStringLiteral("."))
            fixedQualifier.chop(1); //remove last char if it is a dot
        componentName = fixedQualifier + QLatin1Char('.') + componentName;
    }

    if (debug)
        qDebug() << "SubComponentManager" << __FUNCTION__ << componentName;

    if (addToLibrary) {
        // Add file components to the library
        ItemLibraryEntry itemLibraryEntry;
        itemLibraryEntry.setType(componentName.toUtf8());
        itemLibraryEntry.setName(baseComponentName);
        itemLibraryEntry.setCategory(tr("My QML Components"));
        if (!qualifier.isEmpty()) {
            itemLibraryEntry.setRequiredImport(fixedQualifier);
        }

        if (!model()->metaInfo().itemLibraryInfo()->containsEntry(itemLibraryEntry))
            model()->metaInfo().itemLibraryInfo()->addEntries({itemLibraryEntry});
    }
}

Model *SubComponentManager::model() const
{
    return m_model.data();
}

QStringList SubComponentManager::importPaths() const
{
    if (model())
        return model()->importPaths();

    return QStringList();
}

void SubComponentManager::parseQuick3DAssetDir(const QString &assetPath)
{
    QDir assetDir(assetPath);
    const QString assetImportRoot = assetPath.mid(assetPath.lastIndexOf(QLatin1Char('/')) + 1);
    QStringList assets = assetDir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
    for (QString &asset : assets)
        asset.prepend(assetImportRoot + QLatin1Char('.'));

    QStringList newFlowTags;
    const QStringList flowTags = model()->metaInfo().itemLibraryInfo()->showTagsForImports();
    const QString quick3Dlib = QLatin1String(Constants::QT_QUICK_3D_MODULE_NAME);
    const QList<Import> possibleImports = model()->possibleImports();

    auto isPossibleImport = [&possibleImports](const QString &asset) {
        for (const Import &import : possibleImports) {
            if (import.url() == asset)
                return true;
        }
        return false;
    };

    // If there are 3D assets in import path, add a flow tag for QtQuick3D
    if (!assets.isEmpty() && !flowTags.contains(quick3Dlib) && isPossibleImport(quick3Dlib))
        newFlowTags << quick3Dlib;

    // Create item library entries for Quick3D assets that are imported by document
    const QString iconPath = QStringLiteral(":/ItemLibrary/images/item-3D_model-icon.png");
    for (auto &import : qAsConst(m_imports)) {
        if (import.isLibraryImport() && assets.contains(import.url())) {
            assets.removeOne(import.url());
            QDirIterator qmlIt(assetDir.filePath(import.url().split('.').last()),
                               {QStringLiteral("*.qml")}, QDir::Files);
            while (qmlIt.hasNext()) {
                qmlIt.next();
                const QString name = qmlIt.fileInfo().baseName();
                const QString type = import.url() + QLatin1Char('.') + name;
                // For now we assume version is always 1.0 as that's what importer hardcodes
                ItemLibraryEntry itemLibraryEntry;
                itemLibraryEntry.setType(type.toUtf8(), 1, 0);
                itemLibraryEntry.setName(name);
                itemLibraryEntry.setCategory(tr("My Quick3D Components"));
                itemLibraryEntry.setRequiredImport(import.url());
                QString iconName = qmlIt.fileInfo().absolutePath() + '/'
                        + Constants::QUICK_3D_ASSET_ICON_DIR + '/' + name
                        + Constants::QUICK_3D_ASSET_LIBRARY_ICON_SUFFIX;
                if (!QFileInfo(iconName).exists())
                    iconName = iconPath;
                itemLibraryEntry.setLibraryEntryIconPath(iconName);
                itemLibraryEntry.setTypeIcon(QIcon(iconName));

                // load hints file if exists
                QFile hintsFile(qmlIt.fileInfo().absolutePath() + '/' + name + ".hints");
                if (hintsFile.exists() && hintsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&hintsFile);
                    QHash<QString, QString> hints;
                    while (!in.atEnd()) {
                        QStringList hint = in.readLine().split(':');
                        hints.insert(hint[0].trimmed(), hint[1].trimmed());
                    }
                    itemLibraryEntry.addHints(hints);
                }

                model()->metaInfo().itemLibraryInfo()->addEntries({itemLibraryEntry}, true);
            }
        }
    }

    // Create flow tags for the rest, if they are possible imports
    if (!assets.isEmpty()) {
        for (const QString &asset : qAsConst(assets)) {
            if (!flowTags.contains(asset) && isPossibleImport(asset))
                newFlowTags << asset;
        }
    }

    if (!newFlowTags.isEmpty())
        model()->metaInfo().itemLibraryInfo()->addShowTagsForImports(newFlowTags);
}

QStringList SubComponentManager::quick3DAssetPaths() const
{
    const auto impPaths = importPaths();
    QStringList retPaths;
    for (const auto &impPath : impPaths) {
        const QString assetPath = impPath + QLatin1String(Constants::QUICK_3D_ASSETS_FOLDER);
        if (QFileInfo(assetPath).exists())
            retPaths << assetPath;
    }
    return retPaths;
}

/*!
  \class SubComponentManager

  Detects & monitors (potential) component files in a list of directories, and registers
  these in the metatype system.
*/

QStringList SubComponentManager::directories() const
{
    return m_watcher.directories();
}

QStringList SubComponentManager::qmlFiles() const
{
    return m_watcher.files();
}

void SubComponentManager::update(const QUrl &filePath, const QList<Import> &imports)
{
    if (debug)
        qDebug() << Q_FUNC_INFO << filePath << imports.size();

    QFileInfo oldDir, newDir;

    if (!m_filePath.isEmpty()) {
        const QString file = m_filePath.toLocalFile();
        oldDir = QFileInfo(QFileInfo(file).path());
    }
    if (!filePath.isEmpty()) {
        const QString file = filePath.toLocalFile();
        newDir = QFileInfo(QFileInfo(file).path());
    }

    m_filePath = filePath;

    //
    // (implicit) import of local directory
    //
    if (oldDir != newDir) {
        if (!oldDir.filePath().isEmpty()) {
            m_dirToQualifier.remove(oldDir.canonicalFilePath(), QString());
            if (!m_dirToQualifier.contains(oldDir.canonicalFilePath()))
                m_watcher.removePath(oldDir.filePath());

            // Remove old watched asset paths
            const QStringList watchPaths = m_watcher.directories();
            const QString &quick3DAssetFolder = QLatin1String(Constants::QUICK_3D_ASSETS_FOLDER);
            for (const auto &watchPath : watchPaths) {
                if (watchPath.endsWith(quick3DAssetFolder))
                    m_watcher.removePath(watchPath);
            }
        }

        if (!newDir.filePath().isEmpty())
            m_dirToQualifier.insertMulti(newDir.canonicalFilePath(), QString());
    }

    //
    // Imports
    //

    // skip first list items until the lists differ
    int i = 0;
    while (i < qMin(imports.size(), m_imports.size())) {
        if (!(imports.at(i) == m_imports.at(i)))
            break;
        ++i;
    }

    for (int ii = m_imports.size() - 1; ii >= i; --ii)
        removeImport(ii);

    for (int ii = i; ii < imports.size(); ++ii) {
        addImport(ii, imports.at(ii));
    }

    const QString newPath = newDir.absoluteFilePath();
    m_watcher.addPath(newPath);

    // Watch existing asset paths, including a global ones if they exist
    const auto assetPaths = quick3DAssetPaths();
    for (const auto &assetPath : assetPaths)
        m_watcher.addPath(assetPath);

    parseDirectories();
}

} // namespace QmlDesigner

