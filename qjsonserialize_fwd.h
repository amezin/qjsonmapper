/*
 * Copyright © 2015 Alexander Mezin <mezin.alexander@gmail.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#pragma once

class QJsonValue;
class QStringList;

namespace qjsonserialize
{

enum Action
{
    Serialize,
    Deserialize
};

template<Action action, typename Data>
class Context;

template<Action action, typename T>
bool mapValue(const Context<action, T> &ctx);

template<Action action, typename T>
class ObjectContext;

template<Action action, typename T>
void mapObject(ObjectContext<action, T> &);

typedef QStringList ErrorInfo;

template<typename Data>
bool serialize(QJsonValue &json, const Data &data, ErrorInfo &);

template<typename Data>
bool deserialize(const QJsonValue &json, Data &data, ErrorInfo &);

template<Action action>
class ObjectMapping;

}

#define QJSONSERIALIZE_MAPTOJSON_EXTERN(Type) \
    namespace qjsonserialize { \
        extern template bool mapValue<qjsonserialize::Serialize>(const qjsonserialize::Context<qjsonserialize::Serialize, Type> &); \
        extern template bool mapValue<qjsonserialize::Deserialize>(const qjsonserialize::Context<qjsonserialize::Deserialize, Type> &); \
    }

#define QJSONSERIALIZE_MAPTOJSON_OBJECT_EXTERN(Type) \
    namespace qjsonserialize { \
        extern template void mapObject<qjsonserialize::Serialize>(qjsonserialize::ObjectContext<qjsonserialize::Serialize, Type> &); \
        extern template void mapObject<qjsonserialize::Deserialize>(qjsonserialize::ObjectContext<qjsonserialize::Deserialize, Type> &); \
    }

#define QJSONSERIALIZE_MAPTOJSON_OBJECT_MEM_EXTERN(Class) \
    extern template void Class::mapToJson<qjsonserialize::Serialize>(qjsonserialize::ObjectContext<qjsonserialize::Serialize, Class> &); \
    extern template void Class::mapToJson<qjsonserialize::Deserialize>(qjsonserialize::ObjectContext<qjsonserialize::Deserialize, Class> &);

#define QJSONSERIALIZE_MAPTOJSON_MEM_EXTERN(Class) \
    extern template bool Class::mapToJson<qjsonserialize::Serialize>(const qjsonserialize::Context<qjsonserialize::Serialize, Class> &); \
    extern template bool Class::mapToJson<qjsonserialize::Deserialize>(const qjsonserialize::Context<qjsonserialize::Deserialize, Class> &);
