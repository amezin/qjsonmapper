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
    typedef const QJsonObject JsonObject;
};

struct Serialize
{
    typedef QJsonValue Json;
    typedef QJsonObject JsonObject;
};

template<typename Mode, typename T>
struct Serializer;

template<typename T>
bool deserialize(Deserialize::Json &json, T &data)
{
    return Serializer<Deserialize, T>::apply(json, data);
}

template<typename T>
bool serialize(Serialize::Json &json, const T &data)
{
    return Serializer<Serialize, T>::apply(json, data);
}

template<typename Mode, typename T>
struct SerializerTraits;

template<typename T>
struct SerializerTraits<Deserialize, T>
{
    typedef Deserialize::Json Json;
    typedef Deserialize::JsonObject JsonObject;
    typedef T Data;
};

template<typename T>
struct SerializerTraits<Serialize, T>
{
    typedef Serialize::Json Json;
    typedef Serialize::JsonObject JsonObject;
    typedef const T Data;
};

template<typename Mode, typename T>
struct BasicSerializer;

template<typename T>
struct BasicSerializer<Deserialize, T>
{
    static bool apply(Deserialize::Json &json, T &data)
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
    static bool apply(Serialize::Json &json, const T &data)
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
    static bool apply(Deserialize::Json &json, QVariant &data)
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
    static bool apply(Serialize::Json &json, const QVariant &data)
    {
        QJsonValue v(QJsonValue::fromVariant(data));
        if (v.isUndefined()) {
            return false;
        }
        json = v;
        return true;
    }
};

template<>
struct Serializer<Deserialize, QString>
{
    static bool apply(Deserialize::Json &json, QString &data)
    {
        if (json.isUndefined()) {
            return false;
        }
        data = json.toString();
        return true;
    }
};

template<typename Mode, typename Iterator>
struct SequenceSerializer;

template<typename Container>
struct SequenceSerializer<Serialize, Container>
{
    static bool apply(Serialize::Json &json, const Container &data)
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
    static bool apply(Deserialize::Json &json, Container &data)
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

template<typename Mode, typename T, typename Allocator>
struct Serializer<Mode, std::list<T, Allocator> >
        : public SequenceSerializer<Mode, std::list<T, Allocator> > {};

template<typename Mode, typename T, typename Allocator>
struct Serializer<Mode, std::vector<T, Allocator> >
        : public SequenceSerializer<Mode, std::vector<T, Allocator> > {};

template<typename Mode, typename T>
struct Serializer<Mode, QList<T> >
        : public SequenceSerializer<Mode, QList<T> > {};

template<typename Mode>
struct Serializer<Mode, QStringList>
        : public Serializer<Mode, QList<QString> > {};

template<typename Mode, typename T>
struct Serializer<Mode, QVector<T> >
        : public SequenceSerializer<Mode, QVector<T> > {};

template<>
struct Serializer<Serialize, std::string>
{
    static bool apply(Serialize::Json &json, const std::string &data)
    {
        return serialize(json, QString::fromStdString(data));
    }
};

template<>
struct Serializer<Deserialize, std::string>
{
    static bool apply(Deserialize::Json &json, std::string &data)
    {
        QString qstring;
        if (!deserialize(json, qstring)) {
            return false;
        }
        data = qstring.toStdString();
        return true;
    }
};

template<>
struct Serializer<Serialize, std::wstring>
{
    static bool apply(Serialize::Json &json, const std::wstring &data)
    {
        return serialize(json, QString::fromStdWString(data));
    }
};

template<>
struct Serializer<Deserialize, std::wstring>
{
    static bool apply(Deserialize::Json &json, std::wstring &data)
    {
        QString qstring;
        if (!deserialize(json, qstring)) {
            return false;
        }
        data = qstring.toStdWString();
        return true;
    }
};

template<typename Mode, typename T>
struct ObjectSerializer;

template<typename T>
struct ObjectSerializer<Serialize, T>
{
    static bool apply(Serialize::Json &json, const T &data)
    {
        QJsonObject object;
        if (!Serializer<Serialize, T>::map(object, data)) {
            return false;
        }
        json = QJsonValue(object);
        return true;
    }
};

template<typename T>
struct ObjectSerializer<Deserialize, T>
{
    static bool apply(Deserialize::Json &json, T &data)
    {
        if (!json.isObject()) {
            return false;
        }
        return Serializer<Deserialize, T>::map(json.toObject(), data);
    }
};

template<typename Mode, typename T>
struct ObjectInstantiatingSerializer;

template<typename T>
struct ObjectInstantiatingSerializer<Serialize, T>
        : public ObjectSerializer<Serialize, T> {};

template<typename T>
struct ObjectInstantiatingSerializer<Deserialize, T>
{
    static bool apply(Deserialize::Json &json, T &data)
    {
        T newData;
        if (!ObjectSerializer<Deserialize, T>::apply(json, newData)) {
            return false;
        }
        data = newData;
        return true;
    }
};

template<typename V>
bool mapAttribute(Deserialize::JsonObject &o, const QString &name, V &value)
{
    if (!o.contains(name)) {
        return true;
    }
    return deserialize(o.value(name), value);
}

template<typename V>
bool mapAttribute(Serialize::JsonObject &o, const QString &name, const V &value)
{
    if (o.contains(name)) {
        return false;
    }
    QJsonValue attrValue;
    if (!serialize(attrValue, value)) {
        return false;
    }
    o.insert(name, value);
    return true;
}

template<typename V>
bool mapAttribute(Deserialize::JsonObject &o, const QString &name, V &value, const V &defaultValue)
{
    if (!o.contains(name)) {
        value = defaultValue;
        return true;
    }
    return deserialize(o.value(name), value);
}

template<typename V>
bool mapAttribute(Serialize::JsonObject &o, const QString &name, const V &value, const V &)
{
    return mapAttribute(o, name, value);
}

template<typename V>
bool mapAttribute(Deserialize::JsonObject &o, const QString &name, V &value, const QJsonValue &defaultValue)
{
    return deserialize(o.contains(name) ? o.value(name) : defaultValue, value);
}

template<typename V>
bool mapAttribute(Serialize::JsonObject &o, const QString &name, const V &value, const QJsonValue &)
{
    return mapAttribute(o, name, value);
}

template<typename Mode, typename T>
struct PairSerializer : public ObjectInstantiatingSerializer<Mode, T>
{
    static bool map(typename Mode::JsonObject &json,
                    typename SerializerTraits<Mode, T>::Data &data)
    {
        return mapAttribute(json, QStringLiteral("first"), data.first)
                && mapAttribute(json, QStringLiteral("second"), data.second);
    }
};

template<typename Mode, typename First, typename Second>
struct Serializer<Mode, std::pair<First, Second> >
        : public PairSerializer<Mode, std::pair<First, Second> > {};

template<typename Mode, typename First, typename Second>
struct Serializer<Mode, QPair<First, Second> >
        : public PairSerializer<Mode, QPair<First, Second> > {};

}
