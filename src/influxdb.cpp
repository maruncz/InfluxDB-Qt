#include "influxdb.h"
#include "httpinterface.h"

namespace influx
{

InfluxDB::InfluxDB(QObject *parent) : QObject{parent}
{
    iface = std::make_unique<HTTPInterface>(this);
}

InfluxDB::~InfluxDB() = default;

void InfluxDB::write(const QByteArray &bucket, const QVector<Point> &data)
{
    QByteArray arr;
    for (const auto &e : data)
    {
        arr.append(e.toLineProtocol());
    }
    iface->write(bucket, std::move(arr));
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

} // namespace influx
