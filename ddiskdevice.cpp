// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ddiskdevice.h"
#include "udisks2_interface.h"
#include <QDebug>

class DDiskDevicePrivate
{
public:
    OrgFreedesktopUDisks2DriveInterface *dbus = nullptr;
    QDBusError err;
};

DDiskDevice::DDiskDevice(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new DDiskDevicePrivate())
{
    qDebug() << "Creating DDiskDevice for path:" << path;
    d_ptr->dbus = new OrgFreedesktopUDisks2DriveInterface(UDISKS2_SERVICE, path, QDBusConnection::systemBus(), this);
}

DDiskDevice::~DDiskDevice()
{
    qDebug() << "DDiskDevice destroyed:" << path();
}

QString DDiskDevice::path() const
{
    Q_D(const DDiskDevice);
    qDebug() << "Getting disk device path:" << d->dbus->path();
    return d->dbus->path();
}

bool DDiskDevice::canPowerOff() const
{
    qDebug() << "Checking if disk device can power off:" << path();
    return d_ptr->dbus->canPowerOff();
}

QVariantMap DDiskDevice::configuration() const
{
    qDebug() << "Getting disk device configuration:" << path();
    return d_ptr->dbus->configuration();
}

QString DDiskDevice::connectionBus() const
{
    qDebug() << "Getting disk device connection bus:" << path();
    return d_ptr->dbus->connectionBus();
}

bool DDiskDevice::ejectable() const
{
    qDebug() << "Checking if disk device is ejectable:" << path();
    return d_ptr->dbus->ejectable();
}

QString DDiskDevice::id() const
{
    qDebug() << "Getting disk device ID:" << path();
    return d_ptr->dbus->id();
}

QString DDiskDevice::media() const
{
    qDebug() << "Getting disk device media:" << path();
    return d_ptr->dbus->media();
}

bool DDiskDevice::mediaAvailable() const
{
    qDebug() << "Checking if disk device media is available:" << path();
    return d_ptr->dbus->mediaAvailable();
}

bool DDiskDevice::mediaChangeDetected() const
{
    qDebug() << "Checking if disk device media change is detected:" << path();
    return d_ptr->dbus->mediaChangeDetected();
}

QStringList DDiskDevice::mediaCompatibility() const
{
    qDebug() << "Getting disk device media compatibility:" << path();
    return d_ptr->dbus->mediaCompatibility();
}

bool DDiskDevice::mediaRemovable() const
{
    qDebug() << "Checking if disk device media is removable:" << path();
    return d_ptr->dbus->mediaRemovable();
}

QString DDiskDevice::model() const
{
    qDebug() << "Getting disk device model:" << path();
    return d_ptr->dbus->model();
}

bool DDiskDevice::optical() const
{
    qDebug() << "Checking if disk device is optical:" << path();
    return d_ptr->dbus->optical();
}

bool DDiskDevice::opticalBlank() const
{
    qDebug() << "Checking if disk device is optical blank:" << path();
    return d_ptr->dbus->opticalBlank();
}

uint DDiskDevice::opticalNumAudioTracks() const
{
    qDebug() << "Getting disk device optical number of audio tracks:" << path();
    return d_ptr->dbus->opticalNumAudioTracks();
}

uint DDiskDevice::opticalNumDataTracks() const
{
    qDebug() << "Getting disk device optical number of data tracks:" << path();
    return d_ptr->dbus->opticalNumDataTracks();
}

uint DDiskDevice::opticalNumSessions() const
{
    qDebug() << "Getting disk device optical number of sessions:" << path();
    return d_ptr->dbus->opticalNumSessions();
}

uint DDiskDevice::opticalNumTracks() const
{
    qDebug() << "Getting disk device optical number of tracks:" << path();
    return d_ptr->dbus->opticalNumTracks();
}

bool DDiskDevice::removable() const
{
    qDebug() << "Checking if disk device is removable:" << path();
    return d_ptr->dbus->removable();
}

QString DDiskDevice::revision() const
{
    qDebug() << "Getting disk device revision:" << path();
    return d_ptr->dbus->revision();
}

int DDiskDevice::rotationRate() const
{
    qDebug() << "Getting disk device rotation rate:" << path();
    return d_ptr->dbus->rotationRate();
}

QString DDiskDevice::seat() const
{
    qDebug() << "Getting disk device seat:" << path();
    return d_ptr->dbus->seat();
}

QString DDiskDevice::serial() const
{
    qDebug() << "Getting disk device serial:" << path();
    return d_ptr->dbus->serial();
}

QString DDiskDevice::siblingId() const
{
    qDebug() << "Getting disk device sibling ID:" << path();
    return d_ptr->dbus->siblingId();
}

qulonglong DDiskDevice::size() const
{
    qDebug() << "Getting disk device size:" << path();
    return d_ptr->dbus->size();
}

QString DDiskDevice::sortKey() const
{
    qDebug() << "Getting disk device sort key:" << path();
    return d_ptr->dbus->sortKey();
}

qulonglong DDiskDevice::timeDetected() const
{
    qDebug() << "Getting disk device time detected:" << path();
    return d_ptr->dbus->timeDetected();
}

qulonglong DDiskDevice::timeMediaDetected() const
{
    qDebug() << "Getting disk device time media detected:" << path();
    return d_ptr->dbus->timeMediaDetected();
}

QString DDiskDevice::vendor() const
{
    qDebug() << "Getting disk device vendor:" << path();
    return d_ptr->dbus->vendor();
}

QString DDiskDevice::WWN() const
{
    qDebug() << "Getting disk device WWN:" << path();
    return d_ptr->dbus->wWN();
}

QDBusError DDiskDevice::lastError() const
{
    Q_D(const DDiskDevice);
    qDebug() << "Getting disk device last error:" << path();
    return d->err;
}

void DDiskDevice::eject(const QVariantMap &options)
{
    Q_D(DDiskDevice);
    qDebug() << "Ejecting disk device:" << path() << "with options:" << options;
    auto r = d_ptr->dbus->Eject(options);
    r.waitForFinished();
    d->err = r.error();
    if (r.isError()) {
        qWarning() << "Failed to eject disk device:" << path() << "error:" << r.error().message();
    }
}

void DDiskDevice::powerOff(const QVariantMap &options)
{
    Q_D(DDiskDevice);
    qDebug() << "Powering off disk device:" << path() << "with options:" << options;
    auto r = d_ptr->dbus->PowerOff(options);
    r.waitForFinished();
    d->err = r.error();
    if (r.isError()) {
        qWarning() << "Failed to power off disk device:" << path() << "error:" << r.error().message();
    }
}

void DDiskDevice::setConfiguration(const QVariantMap &value, const QVariantMap &options)
{
    Q_D(DDiskDevice);
    qDebug() << "Setting disk device configuration:" << path() << "value:" << value << "options:" << options;
    auto r = d_ptr->dbus->SetConfiguration(value, options);
    r.waitForFinished();
    d->err = r.error();
    if (r.isError()) {
        qWarning() << "Failed to set disk device configuration:" << path() << "error:" << r.error().message();
    }
}
