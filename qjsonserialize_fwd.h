/*
 * Copyright © 2015 Alexander Mezin <mezin.alexander@gmail.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#pragma once

class QJsonObject;
class QJsonValue;

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

template<typename Mode, typename Iterator>
struct SequenceSerializer;

template<typename Mode, typename T>
struct ObjectSerializer;

template<typename Mode, typename T>
struct ObjectInstantiatingSerializer;

template<typename Mode, typename T>
struct PairSerializer;

template<typename Mode, typename Map>
struct MapSerializer;

template<typename Mode, typename Map>
struct StdMapSerializer;

template<typename Mode, typename Map>
struct QtMapSerializer;

}
