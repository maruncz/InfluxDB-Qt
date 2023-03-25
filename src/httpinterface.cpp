#include "httpinterface.h"
#include <QNetworkReply>
#include <QNetworkRequest>

namespace influx
{

HTTPInterface::HTTPInterface(QObject *parent) : QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished, this,
            &HTTPInterface::requestFinished);
}

QNetworkReply *HTTPInterface::query(const QByteArray &bucket,
                                    const QByteArray &query)
{
    QNetworkRequest req;
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                  "application/vnd.flux");
    req.setRawHeader("Accept", "application/csv");
    req.setRawHeader("Authorization", token);
    req.setUrl(QUrl(QString("%1/api/v2/"
                            "query?org=%2&bucket=%3&precision=ms")
                        .arg(url, org, bucket)));

    auto *reply = manager.post(req, query);
    return reply;
}

QNetworkReply *HTTPInterface::write(const QByteArray &bucket, QByteArray &&data)
{
    QNetworkRequest req;
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                  "text/plain; charset=utf-8");
    req.setRawHeader("Authorization", token);
    req.setRawHeader("Accept", "application/json");
    req.setUrl(QUrl(QString("%1/api/v2/"
                            "write?org=%2&bucket=%3&precision=ms")
                        .arg(url, org, bucket)));
    auto *reply = manager.post(req, data);
    return reply;
}

void HTTPInterface::setUrl(const QString &newUrl)
{
    url = newUrl;
}

void HTTPInterface::setOrg(const QString &newOrg)
{
    org = newOrg;
}

void HTTPInterface::setToken(const QByteArray &newToken)
{
    token = QByteArray("Token ") + newToken;
}

} // namespace influx
