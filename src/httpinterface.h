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

    void query(const QByteArray &bucket, const QByteArray &query);
    void write(const QByteArray &bucket, QByteArray &&data);

    void setUrl(const QString &newUrl);
    void setOrg(const QString &newOrg);
    void setToken(const QByteArray &newToken);

signals:
    void requestFinished();
    void requestError(QNetworkReply *);

private slots:
    void replyFinished(QNetworkReply *reply);
    void queryFinished();
    void writeFinished();

private:
    struct writeReq_t
    {
        QNetworkRequest req;
        QByteArray data;
        QNetworkReply *reply;
    };

    void doWrite(writeReq_t *req);
    void advanceWriteQueue();

    QString url;
    QString org;
    QByteArray token;
    QNetworkAccessManager manager{this};
    QQueue<writeReq_t> writeQueue;
    writeReq_t *currentWriteReq{nullptr};
};

} // namespace influx

#endif // HTTPINTERFACE_H
