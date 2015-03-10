/*
 * Copyright © 2015 Alexander Mezin <mezin.alexander@gmail.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#pragma once

#include "qjsonserialize_basic.h"

namespace qjsonserialize
{

template<typename T>
struct ObjectSerializer<Serialize, T>
        : public SerializerTraits<Serialize, T>
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
        : public SerializerTraits<Deserialize, T>
{
    static bool apply(Deserialize::Json &json, T &data)
    {
        if (!json.isObject()) {
            return false;
        }
        return Serializer<Deserialize, T>::map(json.toObject(), data);
    }
};

template<typename T>
struct ObjectInstantiatingSerializer<Serialize, T>
        : public ObjectSerializer<Serialize, T> {};

template<typename T>
struct ObjectInstantiatingSerializer<Deserialize, T>
        : public SerializerTraits<Deserialize, T>
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
    o.insert(name, attrValue);
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

}
