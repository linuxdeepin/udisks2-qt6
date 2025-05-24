// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dudisksjob.h"
#include "udisks2_interface.h"

#include <QDBusConnection>
#include <QDebug>

class DUDisksJobPrivate
{
    DUDisksJobPrivate(DUDisksJob *)
    {

    }
    DUDisksJob *q_ptr;
    OrgFreedesktopUDisks2JobInterface *dbusif;

    Q_DECLARE_PUBLIC(DUDisksJob)
};

DUDisksJob::~DUDisksJob()
{
    qDebug() << "DUDisksJob destroyed:" << path();
}

QString DUDisksJob::path() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job path:" << d->dbusif->path();
    return d->dbusif->path();
}

QStringList DUDisksJob::objects() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job objects for path:" << d->dbusif->path();
    QStringList ret;
    for (auto &o : d->dbusif->objects()) {
        ret.push_back(o.path());
    }
    return ret;
}

bool DUDisksJob::cancelable() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Checking if job is cancelable:" << d->dbusif->path();
    return d->dbusif->cancelable();
}

bool DUDisksJob::progressValid() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Checking if job progress is valid:" << d->dbusif->path();
    return d->dbusif->progressValid();
}

double DUDisksJob::progress() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job progress:" << d->dbusif->path() << "progress:" << d->dbusif->progress();
    return d->dbusif->progress();
}

QString DUDisksJob::operation() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job operation:" << d->dbusif->path() << "operation:" << d->dbusif->operation();
    return d->dbusif->operation();
}

quint32 DUDisksJob::startedByUid() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job started by UID:" << d->dbusif->path();
    return d->dbusif->startedByUID();
}

quint64 DUDisksJob::bytes() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job bytes:" << d->dbusif->path() << "bytes:" << d->dbusif->bytes();
    return d->dbusif->bytes();
}

quint64 DUDisksJob::expectedEndTime() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job expected end time:" << d->dbusif->path();
    return d->dbusif->expectedEndTime();
}

quint64 DUDisksJob::rate() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job rate:" << d->dbusif->path() << "rate:" << d->dbusif->rate();
    return d->dbusif->rate();
}

quint64 DUDisksJob::startTime() const
{
    Q_D(const DUDisksJob);
    qDebug() << "Getting job start time:" << d->dbusif->path();
    return d->dbusif->startTime();
}

void DUDisksJob::cancel(const QVariantMap &options)
{
    Q_D(DUDisksJob);
    qDebug() << "Cancelling job:" << d->dbusif->path() << "with options:" << options;
    auto r = d->dbusif->Cancel(options);
    r.waitForFinished();
    if (r.isError()) {
        qWarning() << "Failed to cancel job:" << d->dbusif->path() << "error:" << r.error().message();
    }
}

DUDisksJob::DUDisksJob(QString path, QObject *parent)
    : QObject(parent)
    , d_ptr(new DUDisksJobPrivate(this))
{
    Q_D(DUDisksJob);
    qDebug() << "Creating DUDisksJob for path:" << path;
    d->dbusif = new OrgFreedesktopUDisks2JobInterface(UDISKS2_SERVICE, path, QDBusConnection::systemBus());
    QDBusConnection::systemBus().connect(UDISKS2_SERVICE, d->dbusif->path(), "org.freedesktop.DBus.Properties",
                   "PropertiesChanged", this, SLOT(onPropertiesChanged(const QString &, const QVariantMap &)));
    connect(d->dbusif, &OrgFreedesktopUDisks2JobInterface::Completed, this, &DUDisksJob::completed);
}

void DUDisksJob::onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties)
{
    Q_UNUSED(interface)
    qDebug() << "Properties changed for job:" << d_ptr->dbusif->path() << "interface:" << interface;

    auto begin = changed_properties.begin();

    for (; begin != changed_properties.constEnd(); ++begin) {
        QString property_name = begin.key();

        int pindex = this->metaObject()->indexOfProperty(property_name.toLatin1().constData());

        if (pindex < 0) {
            property_name[0] = property_name.at(0).toLower();
            pindex = this->metaObject()->indexOfProperty(property_name.toLatin1().constData());
        }

        if (pindex < 0)
            continue;

        const QMetaProperty &mp = this->metaObject()->property(pindex);

        if (!mp.hasNotifySignal())
            continue;

        mp.notifySignal().invoke(this, QGenericArgument(begin.value().typeName(), begin.value().constData()));
    }
}
