#ifndef POINT_H
#define POINT_H

#include <QString>
#include <QVector>
#include <cstdint>
#include <variant>

namespace influx
{

class Point
{
public:
    Point()               = default;
    ~Point()              = default;
    Point(const Point &o) = default;
#if 0
        : measurement(o.measurement), tagSet(o.tagSet), fieldSet(o.fieldSet),
          timestamp(o.timestamp)
    {
    }
#endif

    Point(Point &&o) noexcept
    {
        using std::swap;
        measurement.swap(o.measurement);
        tagSet.swap(o.tagSet);
        fieldSet.swap(o.fieldSet);
        timestamp = o.timestamp;
    }

    Point &operator=(const Point &o) = default;
#if 0
    {
        measurement = o.measurement;
        tagSet      = o.tagSet;
        fieldSet    = o.fieldSet;
        timestamp   = o.timestamp;
        return *this;
    }
#endif

    Point &operator=(Point &&o) noexcept
    {
        using std::swap;
        measurement.swap(o.measurement);
        tagSet.swap(o.tagSet);
        fieldSet.swap(o.fieldSet);
        timestamp = o.timestamp;
        return *this;
    }

    struct Tag
    {
        QString key;
        QString value;
        [[nodiscard]] QByteArray toLineProtocol() const;
    };

    struct Field
    {
        QString key;
        std::variant<double, int64_t, uint64_t, QString, bool> value;
        [[nodiscard]] QByteArray toLineProtocol() const;
    };

    void setMeasurement(QString meas);
    void addTag(Tag t);
    void addField(Field f);
    void setTimestamp(uint64_t time);

    [[nodiscard]] QByteArray toLineProtocol() const;

private:
    QString measurement;
    QVector<Tag> tagSet;
    QVector<Field> fieldSet;
    uint64_t timestamp{0};
};

} // namespace influx

#endif // POINT_H
