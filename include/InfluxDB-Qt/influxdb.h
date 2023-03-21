#ifndef INFLUXDB_H
#define INFLUXDB_H

#include "point.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <memory>

namespace influx
{

class HTTPInterface;

class InfluxDB : public QObject
{
    Q_OBJECT
public:
    explicit InfluxDB(QObject *parent = nullptr);
    ~InfluxDB() override;
    InfluxDB(const InfluxDB &) = delete;
    InfluxDB(InfluxDB &&) = delete;
    InfluxDB& operator=(const InfluxDB &) = delete;
    InfluxDB& operator=(InfluxDB &&) = delete;

    void write(const QByteArray &bucket, const QVector<Point> &data);
    void query(const QByteArray &bucket, const QByteArray &query);

    void setUrl(const QString &newUrl);
    void setOrg(const QString &newOrg);
    void setToken(const QByteArray &newToken);

signals:

private:
    std::unique_ptr<HTTPInterface> iface;
};

} // namespace influx
#endif // INFLUXDB_H