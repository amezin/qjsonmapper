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

#include "qjsonserialize_basic.h"

namespace qjsonserialize
{

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

template<typename T>
struct PairSerializer<Serialize, T>
{
    static bool apply(Serialize::Json &json, const T &data)
    {
        QJsonArray array;
        QJsonValue first, second;
        if (!serialize(first, data.first) || !serialize(second, data.second)) {
            return false;
        }
        array.push_back(first);
        array.push_back(second);
        json = array;
        return true;
    }
};

template<typename T>
struct PairSerializer<Deserialize, T>
{
    static bool apply(Deserialize::Json &json, T &data)
    {
        if (!json.isArray()) {
            return false;
        }
        QJsonArray array(json.toArray());
        if (array.size() != 2) {
            return false;
        }
        T newData;
        if (!deserialize(array[0], newData.first) || !deserialize(array[1], newData.second)) {
            return false;
        }
        data = newData;
        return true;
    }
};

template<typename Key, typename Value>
bool addKeyValueToObject(QJsonObject &object, const Key &key, const Value &value)
{
    QJsonValue keyJson, valueJson;
    if (!serialize(keyJson, key)) {
        return false;
    }
    if (keyJson.isArray() || keyJson.isObject()) {
        return false;
    }
    if (!serialize(valueJson, value)) {
        return false;
    }
    QVariant keyVariant(keyJson.toVariant());
    if (!keyVariant.convert(qMetaTypeId<QString>())) {
        return false;
    }
    QString keyString(keyVariant.toString());
    if (object.contains(keyString)) {
        return false;
    }
    object.insert(keyString, valueJson);
    return true;
}

template<typename Map>
struct StdMapSerializer<Serialize, Map>
{
    static bool apply(Serialize::Json &json, const Map &data)
    {
        QJsonObject object;
        Q_FOREACH(const typename Map::value_type &entry, data) {
            if (!addKeyValueToObject(object, entry.first, entry.second)) {
                return false;
            }
        }
        json = object;
        return true;
    }
};

template<typename Map, typename InsertImplementation>
struct MapDeserializeBase
{
    static bool apply(Deserialize::Json &json, Map &data)
    {
        if (!json.isObject()) {
            return false;
        }
        Map newData;
        QJsonObject object(json.toObject());
        for (QJsonObject::ConstIterator i = object.begin(); i != object.end(); ++i) {
            QJsonValue keyJson(i.key()), valueJson(i.value());
            typename Map::key_type key;
            typename Map::mapped_type value;
            if (!deserialize(keyJson, key)) {
                return false;
            }
            if (!deserialize(valueJson, value)) {
                return false;
            }
            InsertImplementation::insert(newData, key, value);
        }
        data.swap(newData);
        return true;
    }
};

template<typename Map>
struct StdMapSerializer<Deserialize, Map>
        : public MapDeserializeBase<Map, StdMapSerializer<Deserialize, Map> >
{
    static void insert(Map &data, const typename Map::key_type &key, const typename Map::mapped_type &value)
    {
        data.insert(std::make_pair(key, value));
    }
};

template<typename Map>
struct QtMapSerializer<Serialize, Map>
{
    static bool apply(Serialize::Json &json, const Map &data)
    {
        QJsonObject object;
        for (typename Map::ConstIterator i = data.begin(); i != data.end(); ++i) {
            if (!addKeyValueToObject(object, i.key(), i.value())) {
                return false;
            }
        }
        json = object;
        return true;
    }
};

template<typename Map>
struct QtMapSerializer<Deserialize, Map>
        : public MapDeserializeBase<Map, QtMapSerializer<Deserialize, Map> >
{
    static void insert(Map &data, const typename Map::key_type &key, const typename Map::mapped_type &value)
    {
        data.insert(key, value);
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

template<typename Mode, typename First, typename Second>
struct Serializer<Mode, std::pair<First, Second> >
        : public PairSerializer<Mode, std::pair<First, Second> > {};

template<typename Mode, typename First, typename Second>
struct Serializer<Mode, QPair<First, Second> >
        : public PairSerializer<Mode, QPair<First, Second> > {};

template<typename Mode, typename Key, typename Value, typename Predicate, typename Allocator>
struct Serializer<Mode, std::map<Key, Value, Predicate, Allocator> >
        : public StdMapSerializer<Mode, std::map<Key, Value, Predicate, Allocator> > {};

template<typename Mode, typename Key, typename Value>
struct Serializer<Mode, QMap<Key, Value> >
        : public QtMapSerializer<Mode, QMap<Key, Value> > {};

template<typename Mode, typename Key, typename Value>
struct Serializer<Mode, QHash<Key, Value> >
        : public QtMapSerializer<Mode, QHash<Key, Value> > {};

}
