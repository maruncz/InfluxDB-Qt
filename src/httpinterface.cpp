#include "httpinterface.h"
#include <QNetworkReply>
#include <QNetworkRequest>

namespace influx
{

HTTPInterface::HTTPInterface(QObject *parent) : QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished, this,
            &HTTPInterface::replyFinished);
}

void HTTPInterface::query(const QByteArray &bucket, const QByteArray &query)
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
    connect(reply, &QNetworkReply::finished, this,
            &HTTPInterface::queryFinished);
}

void HTTPInterface::write(const QByteArray &bucket, QByteArray &&data)
{
    QNetworkRequest req;
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                  "text/plain; charset=utf-8");
    req.setRawHeader("Authorization", token);
    req.setRawHeader("Accept", "application/json");
    req.setUrl(QUrl(QString("%1/api/v2/"
                            "write?org=%2&bucket=%3&precision=ms")
                        .arg(url, org, bucket)));
    writeQueue.emplaceBack(writeReq_t{req, std::move(data), nullptr});
    if (currentWriteReq == nullptr)
    {
        advanceWriteQueue();
    }
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

void HTTPInterface::replyFinished(QNetworkReply *reply) {}

void HTTPInterface::queryFinished()
{
    /*auto result =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << result;
    if (result == 200)
    {
        emit requestFinished();
    }
    else
    {
        emit requestError(reply);
    }
    reply->deleteLater();*/
}

void HTTPInterface::writeFinished()
{
    auto result = currentWriteReq->reply
                      ->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                      .toInt();
    qDebug() << result;
    if (result == 204)
    {
        emit requestFinished();
        advanceWriteQueue();
    }
    else
    {
        qDebug() << currentWriteReq->reply->readAll();
        emit requestError(currentWriteReq->reply);
    }
    currentWriteReq->reply->deleteLater();
}

void HTTPInterface::doWrite(writeReq_t *req)
{
    req->reply = manager.post(req->req, req->data);
    connect(req->reply, &QNetworkReply::finished, this,
            &HTTPInterface::writeFinished);
}

void HTTPInterface::advanceWriteQueue()
{
    auto &reqest    = writeQueue.head();
    currentWriteReq = &reqest;
    doWrite(currentWriteReq);
}

} // namespace influx
