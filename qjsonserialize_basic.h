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
#include <QVariant>

#include "qjsonserialize_fwd.h"

namespace qjsonserialize
{

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

}
