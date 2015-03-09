/*
 * Copyright © 2015 Alexander Mezin <mezin.alexander@gmail.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#pragma once

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

namespace qjsonserialize
{

struct Deserialize
{
    typedef const QJsonValue Json;
};

struct Serialize
{
    typedef QJsonValue Json;
};

template<typename Mode, typename T>
struct Serializer;

template<typename T>
bool deserialize(Deserialize::Json &json, T &data)
{
    return Serializer<Deserialize, T>()(json, data);
}

template<typename T>
bool serialize(Serialize::Json &json, const T &data)
{
    return Serializer<Serialize, T>()(json, data);
}

template<typename T>
bool map(Deserialize::Json &json, T &data)
{
    return deserialize(json, data);
}

template<typename T>
bool map(Serialize::Json &json, T &data)
{
    return serialize(json, data);
}

template<typename Mode, typename T>
struct SerializerTraits;

template<typename T>
struct SerializerTraits<Deserialize, T>
{
    typedef Deserialize::Json Json;
    typedef T Data;
};

template<typename T>
struct SerializerTraits<Serialize, T>
{
    typedef Serialize::Json Json;
    typedef const T Data;
};

template<typename Mode, typename T>
struct BasicSerializer;

template<typename T>
struct BasicSerializer<Deserialize, T>
{
    bool operator()(Deserialize::Json &json, T &data)
    {
        QVariant asVariant;
        if (!deserialize(json, asVariant)) {
            return false;
        }
        if (!asVariant.convert(qMetaTypeId<T>())) {
            return false;
        }
        data = asVariant.value<T>();
        return true;
    }
};

template<typename T>
struct BasicSerializer<Serialize, T>
{
    bool operator()(Serialize::Json &json, const T &data)
    {
        json = QJsonValue(data);
        return true;
    }
};

template<typename Mode, typename T>
struct Serializer : public BasicSerializer<Mode, T> {};

template<>
struct Serializer<Deserialize, QVariant>
{
    bool operator()(Deserialize::Json &json, QVariant &data)
    {
        if (json.isUndefined()) {
            return false;
        }
        data = json.toVariant();
        return true;
    }
};

template<>
struct Serializer<Serialize, QVariant>
{
    bool operator()(Serialize::Json &json, const QVariant &data)
    {
        QJsonValue v(QJsonValue::fromVariant(data));
        if (v.isUndefined()) {
            return false;
        }
        json = v;
        return true;
    }
};

template<typename Mode, typename Iterator>
struct SequenceSerializer;

template<typename Container>
struct SequenceSerializer<Serialize, Container>
{
    bool operator()(Serialize::Json &json, const Container &data)
    {
        QJsonArray array;
        Q_FOREACH (const typename Container::value_type &v, data) {
            QJsonValue json;
            if (!serialize(json, v)) {
                return false;
            }
            array.append(json);
        }
        json = array;
        return true;
    }
};

template<typename Container>
struct SequenceSerializer<Deserialize, Container>
{
    bool operator()(Deserialize::Json &json, Container &data)
    {
        if (!json.isArray()) {
            return false;
        }
        Container container;
        Q_FOREACH (const QJsonValue &v, json.toArray()) {
            typename Container::value_type item;
            if (!deserialize(v, item)) {
                return false;
            }
            container.push_back(item);
        }
        data.swap(container);
        return true;
    }
};

template<typename T> class QList;
template<typename T> class QVector;

template<typename Mode, typename T, typename Allocator>
struct Serializer<Mode, std::list<T, Allocator> >
        : public SequenceSerializer<Mode, std::list<T, Allocator> > {};

template<typename Mode, typename T, typename Allocator>
struct Serializer<Mode, std::vector<T, Allocator> >
        : public SequenceSerializer<Mode, std::vector<T, Allocator> > {};

template<typename Mode, typename T>
struct Serializer<Mode, QList<T> >
        : public SequenceSerializer<Mode, QList<T> > {};

template<typename Mode, typename T>
struct Serializer<Mode, QVector<T> >
        : public SequenceSerializer<Mode, QVector<T> > {};

}
