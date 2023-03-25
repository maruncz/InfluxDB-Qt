#ifndef HTTPINTERFACE_H
#define HTTPINTERFACE_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QQueue>

namespace influx
{

class HTTPInterface : public QObject
{
    Q_OBJECT
public:
    explicit HTTPInterface(QObject *parent = nullptr);

    QNetworkReply *query(const QByteArray &bucket, const QByteArray &query);
    QNetworkReply *write(const QByteArray &bucket, QByteArray &&data);

    void setUrl(const QString &newUrl);
    void setOrg(const QString &newOrg);
    void setToken(const QByteArray &newToken);

signals:
    void requestFinished(QNetworkReply *);

private:

    QString url;
    QString org;
    QByteArray token;
    QNetworkAccessManager manager{this};
};

} // namespace influx

#endif // HTTPINTERFACE_H
