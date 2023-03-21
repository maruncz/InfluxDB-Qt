#include "point.h"

namespace influx
{

template<class> inline constexpr bool always_false_v = false;

QByteArray Point::Tag::toLineProtocol() const
{
    return (key + '=' + value).toLocal8Bit();
}

QByteArray Point::Field::toLineProtocol() const
{
    auto arr = std::visit(
        [](auto &&arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, double>)
            {
                return QByteArray::number(arg);
            }
            else if constexpr (std::is_same_v<T, int64_t>)
            {
                return QByteArray::number(static_cast<qlonglong>(arg));
            }
            else if constexpr (std::is_same_v<T, uint64_t>)
            {
                return QByteArray::number(static_cast<qulonglong>(arg));
            }
            else if constexpr (std::is_same_v<T, QString>)
            {
                return arg.toLocal8Bit();
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return QByteArray((arg) ? ("T") : ("F"));
            }
            else
            {
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }
        },
        value);
    return (key + '=' + arr).toLocal8Bit();
}

void Point::setMeasurement(QString meas)
{
    measurement.swap(meas);
}

void Point::addTag(Tag t)
{
    tagSet.push_back(std::move(t));
}

void Point::addField(Field f)
{
    fieldSet.push_back(std::move(f));
}

void Point::setTimestamp(uint64_t time)
{
    timestamp = time;
}

QByteArray Point::toLineProtocol() const
{
    QByteArray ret;
    ret.append(measurement.toLocal8Bit());

    for (const auto &e : tagSet)
    {
        ret.append(',');
        ret.append(e.toLineProtocol());
    }

    ret.append(' ');
    for (const auto &e : fieldSet)
    {
        ret.append(e.toLineProtocol());
        ret.append(',');
    }

    if (ret.endsWith(','))
    {
        ret.back() = ' ';
    }

    ret.append(QByteArray::number(static_cast<qlonglong>(timestamp)));

    ret.append('\n');

    return ret;
}

} // namespace influx
