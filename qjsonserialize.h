/*
 * Copyright © 2015 Alexander Mezin <mezin.alexander@gmail.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#pragma once

#include <limits>
#include <cmath>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QVariant>
#include <qnumeric.h>

#include "qjsonserialize_fwd.h"

namespace qjsonserialize
{

template<typename Data>
bool serialize(QJsonValue &json, const Data &data)
{
    return map(Args<Serialize, Data>(json, data));
}

template<typename Data>
bool deserialize(const QJsonValue &json, Data &data)
{
    return map(Args<Deserialize, Data>(json, data));
}

template<typename T>
class Args<Serialize, T>
{
public:
    QJsonValue &json;
    typedef const T &DataRef;
    DataRef data;
    typedef ObjectMapping<Serialize> ObjectMapping;

    Args(QJsonValue &json, const T &data)
        : json(json), data(data)
    {
    }

    Args(const T &data, QJsonValue &json)
        : json(json), data(data)
    {
    }

    Args(const Args &rhs)
        : json(rhs.json), data(rhs.data)
    {
    }

private:
    Args &operator =(const Args &) Q_DECL_EQ_DELETE;
};

template<typename T>
class Args<Deserialize, T>
{
public:
    const QJsonValue &json;
    typedef T &DataRef;
    DataRef data;
    typedef ObjectMapping<Deserialize> ObjectMapping;

    Args(const QJsonValue &json, T &data)
        : json(json), data(data)
    {
    }

    Args(T &data, const QJsonValue &json)
        : json(json), data(data)
    {
    }

    Args(const Args &rhs)
        : json(rhs.json), data(rhs.data)
    {
    }

private:
    Args &operator =(const Args &) Q_DECL_EQ_DELETE;
};

namespace conversions
{

inline bool toQString(const QString &value, QString &out)
{
    out = value;
    return true;
}

inline bool toQString(const QByteArray &value, QString &out)
{
    out = QString::fromUtf8(value);
    return true;
}

inline bool toQString(const std::string &value, QString &out)
{
    out = QString::fromStdString(value);
    return true;
}

inline bool toQString(const std::wstring &value, QString &out)
{
    out = QString::fromStdWString(value);
    return true;
}

inline bool fromQString(const QString &qstring, QString &out)
{
    out = qstring;
    return true;
}

inline bool fromQString(const QString &qstring, QByteArray &out)
{
    out = qstring.toUtf8();
    return true;
}

inline bool fromQString(const QString &qstring, std::string &out)
{
    out = qstring.toStdString();
    return true;
}

inline bool fromQString(const QString &qstring, std::wstring &out)
{
    out = qstring.toStdWString();
    return true;
}

template<typename T>
bool toDouble(T value, double &out)
{
    out = static_cast<double>(value);
    return true;
}

template<typename T>
bool fromDouble(double value, T &out)
{
    if (value > std::numeric_limits<T>::max() || value < std::numeric_limits<T>::min()) {
        return false;
    }
    out = static_cast<T>(value);
    return true;
}

template<>
inline bool fromDouble(double value, double &out)
{
    out = value;
    return true;
}

template<>
inline bool fromDouble(double value, long double &out)
{
    out = value;
    return true;
}

template<>
inline bool fromDouble(double value, float &out)
{
    if (qIsFinite(value) && std::fabs(value) > std::numeric_limits<float>::max()) {
        return false;
    }
    out = static_cast<float>(value);
    return true;
}

}

template<>
class ObjectMapping<Serialize>
{
    Q_DISABLE_COPY(ObjectMapping)

public:
    QJsonValue &jsonValue;
    QJsonObject jsonObject;
    bool good;

    explicit ObjectMapping(QJsonValue &jsonValue)
        : jsonValue(jsonValue), good(true)
    {
    }

    template<typename AttributeType>
    bool map(const QString &key, const AttributeType &value)
    {
        if (!good || jsonObject.contains(key)) {
            return good = false;
        }
        QJsonValue json;
        if (!serialize(json, value)) {
            return good = false;
        }
        jsonObject.insert(key, json);
        return true;
    }

    template<typename AttributeType>
    bool map(const QString &key, const AttributeType &value, const AttributeType &)
    {
        return map(key, value);
    }

    template<typename AttributeType>
    bool map(const QString &key, const AttributeType &value, const QJsonValue &)
    {
        return map(key, value);
    }

    template<typename AttributeType, typename Class, typename Setter>
    bool mapGetSet(const QString &key, const Class &object,
                         AttributeType (Class::*getter)() const,
                         const Setter &)
    {
        return map(key, (object.*getter)());
    }

    template<typename AttributeType, typename Class, typename Setter, typename Default>
    bool mapGetSet(const QString &key, const Class &object,
                         AttributeType (Class::*getter)() const,
                         const Setter &, const Default &)
    {
        return map(key, (object.*getter)());
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, const QObject *object, const char *propertyName)
    {
        QVariant asVariant(object->property(propertyName));
        if (!asVariant.isNull() && !asVariant.convert(qMetaTypeId<AttributeType>())) {
            return good = false;
        }
        return map(key, asVariant.value<AttributeType>());
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, const QObject *object, const char *propertyName, const AttributeType &)
    {
        return mapQProperty<AttributeType>(key, object, propertyName);
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, const QObject *object, const char *propertyName, const QJsonValue &)
    {
        return mapQProperty<AttributeType>(key, object, propertyName);
    }

    ~ObjectMapping()
    {
        if (good) {
            jsonValue = jsonObject;
        }
    }
};

template<>
class ObjectMapping<Deserialize>
{
    Q_DISABLE_COPY(ObjectMapping)

public:
    const QJsonValue &jsonValue;
    QJsonObject jsonObject;
    bool good;

    explicit ObjectMapping(const QJsonValue &jsonValue)
        : jsonValue(jsonValue), jsonObject(jsonValue.toObject()), good(jsonValue.isObject())
    {
    }

    template<typename AttributeType>
    bool map(const QString &key, AttributeType &value)
    {
        if (!good || !jsonObject.contains(key)) {
            return good = false;
        }
        return good = deserialize(jsonObject.value(key), value);
    }

    template<typename AttributeType>
    bool map(const QString &key, AttributeType &value, const AttributeType &defaultValue)
    {
        if (!good) {
            return false;
        }
        if (!jsonObject.contains(key)) {
            value = defaultValue;
            return true;
        }
        return good = deserialize(jsonObject.value(key), value);
    }

    template<typename AttributeType>
    bool map(const QString &key, AttributeType &value, const QJsonValue &defaultJsonValue)
    {
        if (!good) {
            return false;
        }
        return good = deserialize(jsonObject.contains(key) ? jsonObject.value(key) : defaultJsonValue, value);
    }

    template<typename AttributeType, typename Class, typename Getter>
    bool mapGetSet(const QString &key, Class &object, const Getter &,
                         void (Class::*setter)(AttributeType))
    {
        typename RemoveConstRef<AttributeType>::Type value;
        if (map(key, value)) {
            (object.*setter)(value);
        }
        return good;
    }

    template<typename AttributeType, typename Class, typename Getter, typename Default>
    bool mapGetSet(const QString &key, Class &object, const Getter &,
                         void (Class::*setter)(AttributeType),
                         const Default &defaultValue)
    {
        typename RemoveConstRef<AttributeType>::Type value;
        if (map(key, value, defaultValue)) {
            (object.*setter)(value);
        }
        return good;
    }

    template<typename AttributeType, typename Class, typename Getter>
    bool mapGetSet(const QString &key, Class &object, const Getter &,
                         bool (Class::*setter)(AttributeType))
    {
        typename RemoveConstRef<AttributeType>::Type value;
        if (map(key, value)) {
            good = (object->*setter)(value);
        }
        return good;
    }

    template<typename AttributeType, typename Class, typename Getter, typename Default>
    bool mapGetSet(const QString &key, Class &object, const Getter &,
                         bool (Class::*setter)(AttributeType),
                         const Default &defaultValue)
    {
        typename RemoveConstRef<AttributeType>::Type value;
        if (map(key, value, defaultValue)) {
            good = (object->*setter)(value);
        }
        return good;
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, QObject *object, const char *propertyName)
    {
        AttributeType value;
        if (map(key, value)) {
            good = object->setProperty(propertyName, QVariant::fromValue(value));
        }
        return good;
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, QObject *object, const char *propertyName, const AttributeType &defaultValue)
    {
        AttributeType value;
        if (map(key, value, defaultValue)) {
            good = object->setProperty(propertyName, QVariant::fromValue(value));
        }
        return good;
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, QObject *object, const char *propertyName, const QJsonValue &defaultValue)
    {
        AttributeType value;
        if (map(key, value, defaultValue)) {
            good = object->setProperty(propertyName, QVariant::fromValue(value));
        }
        return good;
    }

private:
    template<typename T>
    struct RemoveConstRef
    {
        typedef T Type;
    };

    template<typename T>
    struct RemoveConstRef<const T> : public RemoveConstRef<T> {};

    template<typename T>
    struct RemoveConstRef<T&> : public RemoveConstRef<T> {};
};

template<Action action, typename T>
class ObjectMapArgs : public ObjectMapping<action>
{
    Q_DISABLE_COPY(ObjectMapArgs)

public:

    typename Args<action, T>::DataRef data;

private:

    ObjectMapArgs(const Args<action, T> &args)
        : ObjectMapping<action>(args.json), data(args.data)
    {
    }

    friend bool map<action, T>(const Args<action, T> &);
};

template<Action action, typename T>
void mapObject(ObjectMapArgs<action, T> &);

template<Action action, typename T>
bool map(const Args<action, T> &args)
{
    ObjectMapArgs<action, T> a(args);
    mapObject(a);
    return a.good;
}

namespace implementations
{

template<typename T>
bool stringlike(const Args<Deserialize, T> &args)
{
    if (!args.json.isString()) {
        return false;
    }
    return conversions::fromQString(args.json.toString(), args.data);
}

template<typename T>
bool stringlike(const Args<Serialize, T> &args)
{
    QString asString;
    if (!conversions::toQString(args.data, asString)) {
        return false;
    }
    args.json = QJsonValue(asString);
    return true;
}

template<typename T>
bool boollike(const Args<Deserialize, T> &args)
{
    if (!args.json.isString()) {
        return false;
    }
    args.data = args.json.toBool();
    return true;
}

template<typename T>
bool boollike(const Args<Serialize, T> &args)
{
    args.json = QJsonValue(static_cast<bool>(args.data));
    return true;
}

template<typename T>
bool numeric(const Args<Deserialize, T> &args)
{
    if (!args.json.isDouble()) {
        return false;
    }
    return conversions::fromDouble(args.json.toDouble(), args.data);
}

template<typename T>
bool numeric(const Args<Serialize, T> &args)
{
    double value;
    if (!conversions::toDouble(args.data, value)) {
        return false;
    }
    args.json = QJsonValue(value);
    return true;
}

}

template<Action action>
inline bool map(const Args<action, bool> &args)
{
    return implementations::boollike(args);
}

template<Action action>
inline bool map(const Args<action, QString> &args)
{
    return implementations::stringlike(args);
}

template<Action action>
inline bool map(const Args<action, QByteArray> &args)
{
    return implementations::stringlike(args);
}

template<Action action>
inline bool map(const Args<action, std::string> &args)
{
    return implementations::stringlike(args);
}

template<Action action>
inline bool map(const Args<action, std::wstring> &args)
{
    return implementations::stringlike(args);
}

template<Action action>
bool map(const Args<action, int> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, short> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, long long> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, unsigned int> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, unsigned short> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, unsigned long long> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, float> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, double> &args)
{
    return implementations::numeric(args);
}

template<Action action>
bool map(const Args<action, long double> &args)
{
    return implementations::numeric(args);
}

template<>
inline bool map(const Args<Serialize, QVariant> &args)
{
    args.json = QJsonValue::fromVariant(args.data);
    return true;
}

template<>
inline bool map(const Args<Deserialize, QVariant> &args)
{
    args.data = args.json.toVariant();
    return true;
}

namespace implementations
{

template<typename Container>
bool sequence(const Args<Serialize, Container> &args)
{
    QJsonArray array;
    Q_FOREACH (const typename Container::value_type &v, args.data) {
        QJsonValue json;
        if (!serialize(json, v)) {
            return false;
        }
        array.append(json);
    }
    args.json = array;
    return true;
}

template<typename Container>
bool sequence(const Args<Deserialize, Container> &args)
{
    if (!args.json.isArray()) {
        return false;
    }
    Container container;
    Q_FOREACH (const QJsonValue &v, args.json.toArray()) {
        typename Container::value_type item;
        if (!deserialize(v, item)) {
            return false;
        }
        container.push_back(item);
    }
    args.data.swap(container);
    return true;
}

}

template<Action action, typename T, typename Allocator>
bool map(const Args<action, std::list<T, Allocator> > &args)
{
    return implementations::sequence(args);
}

template<Action action, typename T, typename Allocator>
bool map(const Args<action, std::vector<T, Allocator> > &args)
{
    return implementations::sequence(args);
}

template<Action action, typename T>
bool map(const Args<action, QList<T> > &args)
{
    return implementations::sequence(args);
}

template<Action action, typename T>
bool map(const Args<action, QVector<T> > &args)
{
    return implementations::sequence(args);
}

template<Action action>
bool map(const Args<action, QStringList> &args)
{
    return implementations::sequence(args);
}

namespace implementations
{

template<typename Pair>
bool pair(const Args<Serialize, Pair> &args)
{
    QJsonArray array;
    QJsonValue first, second;
    if (!serialize(first, args.data.first) ||
            !serialize(second, args.data.second))
    {
        return false;
    }
    array.push_back(first);
    array.push_back(second);
    args.json = array;
    return true;
}

template<typename Pair>
bool pair(const Args<Deserialize, Pair> &args)
{
    if (!args.json.isArray()) {
        return false;
    }
    QJsonArray array(args.json.toArray());
    if (array.size() != 2) {
        return false;
    }
    Pair newData;
    if (!deserialize(array[0], newData.first) || !deserialize(array[1], newData.second)) {
        return false;
    }
    args.data = newData;
    return true;
}

}

template<Action action, typename First, typename Second>
bool map(const Args<action, std::pair<First, Second> > &args)
{
    return implementations::pair(args);
}

template<Action action, typename First, typename Second>
bool map(const Args<action, QPair<First, Second> > &args)
{
    return implementations::pair(args);
}

namespace implementations
{

template<typename Map>
struct StdMapTraits
{
    typedef Map Container;
    typedef typename Map::const_iterator Iterator;
    typedef typename Map::key_type KeyType;
    typedef typename Map::mapped_type ValueType;

    template<typename Iterator>
    static KeyType key(const Iterator &i)
    {
        return i->first;
    }

    template<typename Iterator>
    static ValueType value(const Iterator &i)
    {
        return i->second;
    }

    static void insert(Map &map, const KeyType &key, const ValueType &value)
    {
        map.insert(std::pair<KeyType, ValueType>(key, value));
    }
};

template<typename Map>
struct QtMapTraits
{
    typedef Map Container;
    typedef typename Map::const_iterator Iterator;
    typedef typename Map::key_type KeyType;
    typedef typename Map::mapped_type ValueType;

    template<typename Iterator>
    static KeyType key(const Iterator &i)
    {
        return i.key();
    }

    template<typename Iterator>
    static ValueType value(const Iterator &i)
    {
        return i.value();
    }

    static void insert(Map &map, const KeyType &key, const ValueType &value)
    {
        map.insert(key, value);
    }
};

template<typename Traits>
bool associative(const Args<Serialize, typename Traits::Container> &args)
{
    QJsonObject object;
    for (typename Traits::Iterator i = args.data.begin(); i != args.data.end(); ++i) {
        QString keyString;
        if (!conversions::toQString(Traits::key(i), keyString)) {
            return false;
        }
        if (object.contains(keyString)) {
            return false;
        }
        QJsonValue valueJson;
        if (!serialize(valueJson, Traits::value(i))) {
            return false;
        }
        object.insert(keyString, valueJson);
    }
    args.json = object;
    return true;
}

template<typename Traits>
bool associative(const Args<Deserialize, typename Traits::Container> &args)
{
    if (!args.json.isObject()) {
        return false;
    }
    typename Traits::Container newData;
    QJsonObject object(args.json.toObject());
    for (QJsonObject::ConstIterator i = object.begin(); i != object.end(); ++i) {
        typename Traits::KeyType key;
        if (!conversions::fromQString(i.key(), key)) {
            return false;
        }
        if (newData.count(key)) {
            return false;
        }
        typename Traits::ValueType value;
        if (!deserialize(i.value(), value)) {
            return false;
        }
        Traits::insert(newData, key, value);
    }
    args.data.swap(newData);
    return true;
}

}

template<Action action, typename Key, typename Value>
bool map(const Args<action, QMap<Key, Value> > &args)
{
    return implementations::associative<implementations::QtMapTraits<QMap<Key, Value> > >(args);
}

template<Action action, typename Key, typename Value>
bool map(const Args<action, QHash<Key, Value> > &args)
{
    return implementations::associative<implementations::QtMapTraits<QHash<Key, Value> > >(args);
}

template<Action action, typename Key, typename Value, typename Predicate, typename Allocator>
bool map(const Args<action, std::map<Key, Value, Predicate, Allocator> > &args)
{
    return implementations::associative<implementations::StdMapTraits<std::map<Key, Value, Predicate, Allocator> > >(args);
}

}
