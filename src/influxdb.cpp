#include "influxdb.h"
#include "httpinterface.h"
#include <QDebug>
#include <QNetworkReply>

namespace influx
{

InfluxDB::InfluxDB(QObject *parent) : QObject{parent}
{
    iface = std::make_unique<HTTPInterface>(this);
    connect(iface.get(), &HTTPInterface::requestFinished, this,
            &InfluxDB::requestFinished);
}

InfluxDB::~InfluxDB() = default;

QNetworkReply *InfluxDB::write(const QByteArray &bucket,
                               const QVector<Point> &data)
{
    QByteArray arr;
    for (const auto &e : data)
    {
        arr.append(e.toLineProtocol());
    }
    return iface->write(bucket, arr);
}

void InfluxDB::query(const QByteArray &bucket, const QByteArray &query)
{
    iface->query(bucket, query);
}

void InfluxDB::setUrl(const QString &newUrl)
{
    iface->setUrl(newUrl);
}

void InfluxDB::setOrg(const QString &newOrg)
{
    iface->setOrg(newOrg);
}

void InfluxDB::setToken(const QByteArray &newToken)
{
    iface->setToken(newToken);
}

void InfluxDB::setTimeout(int msec)
{
    iface->setTimeout(msec);
}

} // namespace influx
