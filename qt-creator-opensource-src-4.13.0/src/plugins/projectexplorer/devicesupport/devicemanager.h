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

#include "idevice.h"

#include <projectexplorer/projectexplorer_export.h>

#include <QObject>

#include <memory>

namespace Utils { class FilePath; }

namespace ProjectExplorer {
class IDevice;
class IDeviceFactory;

class ProjectExplorerPlugin;

namespace Internal {
class DeviceManagerPrivate;
class DeviceSettingsWidget;
} // namespace Internal

class PROJECTEXPLORER_EXPORT DeviceManager : public QObject
{
    Q_OBJECT
    friend class Internal::DeviceSettingsWidget;
    friend class IDevice;

public:
    ~DeviceManager() override;

    static DeviceManager *instance();

    int deviceCount() const;
    IDevice::ConstPtr deviceAt(int index) const;

    IDevice::ConstPtr find(Utils::Id id) const;
    IDevice::ConstPtr defaultDevice(Utils::Id deviceType) const;
    bool hasDevice(const QString &name) const;

    void addDevice(const IDevice::ConstPtr &device);
    void removeDevice(Utils::Id id);
    void setDeviceState(Utils::Id deviceId, IDevice::DeviceState deviceState);

    bool isLoaded() const;

signals:
    void deviceAdded(Utils::Id id);
    void deviceRemoved(Utils::Id id);
    void deviceUpdated(Utils::Id id);
    void deviceListReplaced(); // For bulk changes via the settings dialog.
    void updated(); // Emitted for all of the above.

    void devicesLoaded(); // Emitted once load() is done

private:
    void save();

    DeviceManager(bool isInstance = true);

    void load();
    static const IDeviceFactory *restoreFactory(const QVariantMap &map);
    QList<IDevice::Ptr> fromMap(const QVariantMap &map, QHash<Utils::Id, Utils::Id> *defaultDevices);
    QVariantMap toMap() const;

    // For SettingsWidget.
    IDevice::Ptr mutableDevice(Utils::Id id) const;
    void setDefaultDevice(Utils::Id id);
    static DeviceManager *cloneInstance();
    static void replaceInstance();
    static void removeClonedInstance();

    static Utils::FilePath settingsFilePath(const QString &extension);
    static Utils::FilePath systemSettingsFilePath(const QString &deviceFileRelativePath);
    static void copy(const DeviceManager *source, DeviceManager *target, bool deep);

    const std::unique_ptr<Internal::DeviceManagerPrivate> d;

    static DeviceManager *m_instance;

    friend class Internal::DeviceManagerPrivate;
    friend class ProjectExplorerPlugin;
    friend class ProjectExplorerPluginPrivate;
};

} // namespace ProjectExplorer
