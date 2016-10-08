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

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMetaEnum>
#include <QString>
#include <QVariant>
#include <qnumeric.h>

#include "qjsonmapper_fwd.h"

namespace qjsonmapper
{

template<typename Data>
bool serialize(QJsonValue &json, const Data &data, ErrorInfo &errorInfo)
{
    Context<Serialize, Data> ctx(json, data, errorInfo);
    return mapValue(ctx);
}

template<typename Data>
bool deserialize(const QJsonValue &json, Data &data, ErrorInfo &errorInfo)
{
    Context<Deserialize, Data> ctx(json, data, errorInfo);
    return mapValue(ctx);
}

template<typename T>
class Context<Serialize, T>
{
public:
    typedef QJsonValue &JsonRef;
    JsonRef json;
    typedef const T &DataRef;
    DataRef data;
    typedef ObjectMapping<Serialize> ObjectMappingType;
    ErrorInfo &errorInfo;

    Context(QJsonValue &json, const T &data, ErrorInfo &errorInfo)
        : json(json), data(data), errorInfo(errorInfo)
    {
    }

    template<typename T2>
    explicit Context(const Context<Serialize, T2> &rhs)
        : json(rhs.json), data(rhs.data), errorInfo(rhs.errorInfo)
    {
    }
};

template<typename T>
class Context<Deserialize, T>
{
public:
    typedef const QJsonValue &JsonRef;
    JsonRef json;
    typedef T &DataRef;
    DataRef data;
    typedef ObjectMapping<Deserialize> ObjectMappingType;
    ErrorInfo &errorInfo;

    Context(const QJsonValue &json, T &data, ErrorInfo &errorInfo)
        : json(json), data(data), errorInfo(errorInfo)
    {
    }

    template<typename T2>
    explicit Context(const Context<Deserialize, T2> &rhs)
        : json(rhs.json), data(rhs.data), errorInfo(rhs.errorInfo)
    {
    }
};

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

template<Action action>
class ObjectMappingInterface;

template<>
class ObjectMappingInterface<Serialize>
{
public:
    QJsonObject &jsonObject;
    bool &good;
    ErrorInfo &errorInfo;

    ObjectMappingInterface(QJsonObject &jsonObject, bool &good, ErrorInfo &errorInfo)
        : jsonObject(jsonObject), good(good), errorInfo(errorInfo)
    {
    }

    template<typename AttributeType>
    bool mapField(const QString &key, const AttributeType &value)
    {
        if (!good) {
            return false;
        }
        if (jsonObject.contains(key)) {
            errorInfo.append(QStringLiteral("Duplicate attribute '%1'").arg(key));
            return good = false;
        }
        QJsonValue json;
        if (!serialize(json, value, errorInfo)) {
            errorInfo.append(QStringLiteral("Can't generate JSON for attribute '%1'").arg(key));
            return good = false;
        }
        jsonObject.insert(key, json);
        return true;
    }

    template<typename AttributeType>
    bool mapField(const QString &key, const AttributeType &value, const AttributeType &)
    {
        return mapField(key, value);
    }

    template<typename AttributeType>
    bool mapField(const QString &key, const AttributeType &value, const QJsonValue &)
    {
        return mapField(key, value);
    }

    template<typename AttributeType, typename Class, typename SetterReturn, typename SetterArg>
    bool mapGetSet(const QString &key, const Class &object,
                   AttributeType (Class::*getter)() const,
                   SetterReturn (Class::*)(SetterArg))
    {
        return mapField(key, (object.*getter)());
    }

    template<typename AttributeType, typename Class, typename SetterReturn, typename SetterArg, typename Default>
    bool mapGetSet(const QString &key, const Class &object,
                   AttributeType (Class::*getter)() const,
                   SetterReturn (Class::*)(SetterArg),
                   const Default &)
    {
        return mapField(key, (object.*getter)());
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, const QObject *object, const char *propertyName)
    {
        if (!good) {
            return false;
        }
        QVariant asVariant(object->property(propertyName));
        if (!asVariant.convert(qMetaTypeId<AttributeType>())) {
            QString errorMessage;
            QDebug(&errorMessage) << "Can't convert" << asVariant
                                  << "to" << QMetaType::typeName(qMetaTypeId<AttributeType>())
                                  << "(when parsing" << key << ")";
            errorInfo.append(errorMessage);
            return good = false;
        }
        return mapField(key, asVariant.value<AttributeType>());
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

    template<typename AttributeType>
    bool mapQProperty(const QObject *object, const char *propertyName)
    {
        return mapQProperty<AttributeType>(QLatin1String(propertyName), object, propertyName);
    }

    template<typename AttributeType>
    bool mapQProperty(const QObject *object, const char *propertyName, const AttributeType &)
    {
        return mapQProperty<AttributeType>(object, propertyName);
    }

    template<typename AttributeType>
    bool mapQProperty(const QObject *object, const char *propertyName, const QJsonValue &)
    {
        return mapQProperty<AttributeType>(object, propertyName);
    }

};

namespace util
{

template<typename T>
struct RemoveConstRef
{
    typedef T Type;
};

template<typename T>
struct RemoveConstRef<const T> : public RemoveConstRef<T> {};

template<typename T>
struct RemoveConstRef<T&> : public RemoveConstRef<T> {};

}

template<>
class ObjectMappingInterface<Deserialize>
{
public:
    const QJsonObject &jsonObject;
    bool &good;
    ErrorInfo &errorInfo;

    ObjectMappingInterface(const QJsonObject &jsonObject, bool &good, ErrorInfo &errorInfo)
        : jsonObject(jsonObject), good(good), errorInfo(errorInfo)
    {
    }

    template<typename AttributeType>
    bool mapField(const QString &key, AttributeType &value)
    {
        if (!good) {
            return false;
        }
        if (!jsonObject.contains(key)) {
            errorInfo.append(QStringLiteral("JSON object has no attribute '%1'").arg(key));
            return good = false;
        }
        return parseAttr(key, jsonObject.value(key), value);
    }

    template<typename AttributeType>
    bool mapField(const QString &key, AttributeType &value, const AttributeType &defaultValue)
    {
        if (!good) {
            return false;
        }
        if (!jsonObject.contains(key)) {
            value = defaultValue;
            return true;
        }
        return parseAttr(key, jsonObject.value(key), value);
    }

    template<typename AttributeType>
    bool mapField(const QString &key, AttributeType &value, const QJsonValue &defaultJsonValue)
    {
        if (!good) {
            return false;
        }
        return parseAttr(key, jsonObject.contains(key) ? jsonObject.value(key) : defaultJsonValue, value);
    }

    template<typename AttributeType, typename Class, typename GetterReturn>
    bool mapGetSet(const QString &key, Class &object,
                   GetterReturn (Class::*)() const,
                   void (Class::*setter)(AttributeType))
    {
        typename util::RemoveConstRef<AttributeType>::Type value;
        if (mapField(key, value)) {
            (object.*setter)(value);
        }
        return good;
    }

    template<typename AttributeType, typename Class, typename GetterReturn, typename Default>
    bool mapGetSet(const QString &key, Class &object,
                   GetterReturn (Class::*)() const,
                   void (Class::*setter)(AttributeType),
                   const Default &defaultValue)
    {
        typename util::RemoveConstRef<AttributeType>::Type value;
        if (mapField(key, value, defaultValue)) {
            (object.*setter)(value);
        }
        return good;
    }

    template<typename AttributeType, typename Class, typename GetterReturn>
    bool mapGetSet(const QString &key, Class &object,
                   GetterReturn (Class::*)() const,
                   bool (Class::*setter)(AttributeType))
    {
        typename util::RemoveConstRef<AttributeType>::Type value;
        if (mapField(key, value)) {
            good = (object->*setter)(value);
            checkSetterResult(key);
        }
        return good;
    }

    template<typename AttributeType, typename Class, typename GetterReturn, typename Default>
    bool mapGetSet(const QString &key, Class &object,
                   GetterReturn (Class::*)() const,
                   bool (Class::*setter)(AttributeType),
                   const Default &defaultValue)
    {
        typename util::RemoveConstRef<AttributeType>::Type value;
        if (mapField(key, value, defaultValue)) {
            good = (object->*setter)(value);
            checkSetterResult(key);
        }
        return good;
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, QObject *object, const char *propertyName)
    {
        AttributeType value;
        if (mapField(key, value)) {
            good = object->setProperty(propertyName, QVariant::fromValue(value));
            checkSetterResult(key);
        }
        return good;
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, QObject *object, const char *propertyName, const AttributeType &defaultValue)
    {
        AttributeType value;
        if (mapField(key, value, defaultValue)) {
            good = object->setProperty(propertyName, QVariant::fromValue(value));
            checkSetterResult(key);
        }
        return good;
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, QObject *object, const char *propertyName, const QJsonValue &defaultValue)
    {
        AttributeType value;
        if (mapField(key, value, defaultValue)) {
            good = object->setProperty(propertyName, QVariant::fromValue(value));
            checkSetterResult(key);
        }
        return good;
    }

    template<typename AttributeType>
    bool mapQProperty(QObject *object, const char *propertyName)
    {
        return mapQProperty<AttributeType>(QLatin1String(propertyName), object, propertyName);
    }

    template<typename AttributeType>
    bool mapQProperty(QObject *object, const char *propertyName, const AttributeType &defaultValue)
    {
        return mapQProperty<AttributeType>(QLatin1String(propertyName), object, propertyName, defaultValue);
    }

    template<typename AttributeType>
    bool mapQProperty(QObject *object, const char *propertyName, const QJsonValue &defaultValue)
    {
        return mapQProperty<AttributeType>(QLatin1String(propertyName), object, propertyName, defaultValue);
    }

private:
    template<typename AttributeType>
    bool parseAttr(const QString &key, const QJsonValue &json, AttributeType &value)
    {
        good = deserialize(json, value, errorInfo);
        if (!good) {
            errorInfo.append(QStringLiteral("Can't parse attribute '%1'").arg(key));
        }
        return good;
    }

    bool checkSetterResult(const QString &key)
    {
        if (!good) {
            errorInfo.append(QStringLiteral("Can't set value for '%1'").arg(key));
        }
        return good;
    }
};

template<>
class ObjectMapping<Serialize> : public ObjectMappingInterface<Serialize>
{
public:
    QJsonValue &jsonValue;
    QJsonObject jsonObject;
    bool good;

    using ObjectMappingInterface<Serialize>::mapField;
    using ObjectMappingInterface<Serialize>::mapGetSet;
    using ObjectMappingInterface<Serialize>::mapQProperty;

    explicit ObjectMapping(QJsonValue &jsonValue, ErrorInfo &errorInfo)
        : ObjectMappingInterface<Serialize>(jsonObject, good, errorInfo),
          jsonValue(jsonValue),
          good(true)
    {
    }

    ~ObjectMapping()
    {
        if (good) {
            jsonValue = jsonObject;
        }
    }
};

template<>
class ObjectMapping<Deserialize> : public ObjectMappingInterface<Deserialize>
{
public:
    const QJsonValue &jsonValue;
    QJsonObject jsonObject;
    bool good;

    using ObjectMappingInterface<Deserialize>::mapField;
    using ObjectMappingInterface<Deserialize>::mapGetSet;
    using ObjectMappingInterface<Deserialize>::mapQProperty;

    explicit ObjectMapping(const QJsonValue &jsonValue, ErrorInfo &errorInfo)
        : ObjectMappingInterface<Deserialize>(jsonObject, good, errorInfo),
          jsonValue(jsonValue),
          jsonObject(jsonValue.toObject()),
          good(jsonValue.isObject())
    {
    }
};

template<Action action, typename T>
class ObjectContext : public ObjectMappingInterface<action>
{
public:
    typename Context<action, T>::DataRef data;

    using ObjectMappingInterface<action>::mapField;
    using ObjectMappingInterface<action>::mapGetSet;
    using ObjectMappingInterface<action>::mapQProperty;

    template<typename GetterReturn, typename SetterReturn, typename SetterArgument>
    bool mapGetSet(const QString &key,
                   GetterReturn (T::*getter)() const,
                   SetterReturn (T::*setter)(SetterArgument))
    {
        return ObjectMappingInterface<action>::mapGetSet(key, data, getter, setter);
    }

    template<typename GetterReturn, typename SetterReturn, typename SetterArgument, typename Default>
    bool mapGetSet(const QString &key,
                   GetterReturn (T::*getter)() const,
                   SetterReturn (T::*setter)(SetterArgument),
                   const Default &defaultValue)
    {
        return ObjectMappingInterface<action>::mapGetSet(key, data, getter, setter, defaultValue);
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, const char *propertyName)
    {
        return ObjectMappingInterface<action>::template mapQProperty<AttributeType>(key, &data, propertyName);
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, const char *propertyName, const AttributeType &defaultValue)
    {
        return ObjectMappingInterface<action>::template mapQProperty<AttributeType>(key, &data, propertyName, defaultValue);
    }

    template<typename AttributeType>
    bool mapQProperty(const QString &key, const char *propertyName, const QJsonValue &defaultValue)
    {
        return ObjectMappingInterface<action>::template mapQProperty<AttributeType>(key, &data, propertyName, defaultValue);
    }

    template<typename AttributeType>
    bool mapQProp(const char *propertyName)
    {
        return ObjectMappingInterface<action>::template mapQProperty<AttributeType>(&data, propertyName);
    }

    template<typename AttributeType>
    bool mapQProp(const char *propertyName, const AttributeType &defaultValue)
    {
        return ObjectMappingInterface<action>::template mapQProperty<AttributeType>(&data, propertyName, defaultValue);
    }

    template<typename AttributeType>
    bool mapQProp(const char *propertyName, const QJsonValue &defaultValue)
    {
        return ObjectMappingInterface<action>::template mapQProperty<AttributeType>(&data, propertyName, defaultValue);
    }

    ObjectContext(typename Context<action, T>::DataRef data, const ObjectMappingInterface<action> &mapping)
        : ObjectMappingInterface<action>(mapping.jsonObject, mapping.good, mapping.errorInfo), data(data)
    {
    }

    template<typename T2>
    explicit ObjectContext(const ObjectContext<action, T2> &copy)
        : ObjectMappingInterface<action>(copy.jsonObject, copy.good, copy.errorInfo), data(copy.data)
    {
    }
};

template<Action action, typename T>
void mapObject(ObjectContext<action, T> &);

namespace util
{
template<typename Return, typename Arg, Return (*)(Arg)> struct FunctionExists;
}

template<Action action, typename T>
bool mapValueImpl(const Context<action, T> &ctx,
                  util::FunctionExists<bool, const Context<action, T> &, &T::mapToJson> *)
{
    return T::mapToJson(ctx);
}

template<Action action, typename T>
bool mapValueImpl(const Context<action, T> &ctx,
                  util::FunctionExists<void, ObjectContext<action, T> &, &T::mapToJson> *)
{
    ObjectMapping<action> mapping(ctx.json, ctx.errorInfo);
    ObjectContext<action, T> o(ctx.data, mapping);
    T::mapToJson(o);
    return mapping.good;
}

template<Action action, typename T>
bool mapValueImpl(const Context<action, T> &ctx, ...)
{
    ObjectMapping<action> mapping(ctx.json, ctx.errorInfo);
    ObjectContext<action, T> o(ctx.data, mapping);
    mapObject(o);
    return mapping.good;
}

template<Action action, typename T>
bool mapValue(const Context<action, T> &ctx)
{
    return mapValueImpl<action, T>(ctx, Q_NULLPTR);
}

template<typename T>
bool mapString(const Context<Deserialize, T> &ctx)
{
    if (!ctx.json.isString()) {
        QString errorMessage;
        QDebug(&errorMessage) << "String expected instead of" << ctx.json;
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    if (fromQString(ctx.json.toString(), ctx.data)) {
        return true;
    }
    QString errorMessage;
    QDebug(&errorMessage) << "String conversion failed for" << ctx.json;
    ctx.errorInfo.append(errorMessage);
    return false;
}

template<typename T>
bool mapString(const Context<Serialize, T> &ctx)
{
    QString asString;
    if (!toQString(ctx.data, asString)) {
        ctx.errorInfo.append(QStringLiteral("Can't convert data to string"));
        return false;
    }
    ctx.json = QJsonValue(asString);
    return true;
}

template<typename T>
bool mapBool(const Context<Deserialize, T> &ctx)
{
    if (!ctx.json.isBool()) {
        QString errorMessage;
        QDebug(&errorMessage) << "Boolean expected instead of" << ctx.json;
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    ctx.data = ctx.json.toBool();
    return true;
}

template<typename T>
bool mapBool(const Context<Serialize, T> &ctx)
{
    ctx.json = QJsonValue(static_cast<bool>(ctx.data));
    return true;
}

template<typename T>
bool mapNumeric(const Context<Deserialize, T> &ctx)
{
    if (!ctx.json.isDouble()) {
        QString errorMessage;
        QDebug(&errorMessage) << "Number expected instead of" << ctx.json;
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    if (fromDouble(ctx.json.toDouble(), ctx.data)) {
        return true;
    }
    QString errorMessage;
    QDebug(&errorMessage) << "Number conversion failed for" << ctx.json.toDouble();
    ctx.errorInfo.append(errorMessage);
    return false;
}

template<typename T>
bool mapNumeric(const Context<Serialize, T> &ctx)
{
    double value;
    if (!toDouble(ctx.data, value)) {
        ctx.errorInfo.append(QStringLiteral("Can't convert data to number"));
        return false;
    }
    ctx.json = QJsonValue(value);
    return true;
}

template<Action action>
inline bool mapValue(const Context<action, bool> &ctx)
{
    return mapBool(ctx);
}

template<Action action>
inline bool mapValue(const Context<action, QString> &ctx)
{
    return mapString(ctx);
}

template<Action action>
inline bool mapValue(const Context<action, QByteArray> &ctx)
{
    return mapString(ctx);
}

template<Action action>
inline bool mapValue(const Context<action, std::string> &ctx)
{
    return mapString(ctx);
}

template<Action action>
inline bool mapValue(const Context<action, std::wstring> &ctx)
{
    return mapString(ctx);
}

template<Action action>
bool mapValue(const Context<action, int> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, short> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, long long> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, unsigned int> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, unsigned short> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, unsigned long long> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, float> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, double> &ctx)
{
    return mapNumeric(ctx);
}

template<Action action>
bool mapValue(const Context<action, long double> &ctx)
{
    return mapNumeric(ctx);
}

template<>
inline bool mapValue(const Context<Serialize, QVariant> &ctx)
{
    ctx.json = QJsonValue::fromVariant(ctx.data);
    return true;
}

template<>
inline bool mapValue(const Context<Deserialize, QVariant> &ctx)
{
    ctx.data = ctx.json.toVariant();
    return true;
}

template<typename Container>
bool mapSequence(const Context<Serialize, Container> &ctx)
{
    QJsonArray array;
    Q_FOREACH (const typename Container::value_type &v, ctx.data) {
        QJsonValue json;
        if (!serialize(json, v, ctx.errorInfo)) {
            return false;
        }
        array.append(json);
    }
    ctx.json = array;
    return true;
}

template<typename Container>
bool mapSequence(const Context<Deserialize, Container> &ctx)
{
    if (!ctx.json.isArray()) {
        QString errorMessage;
        QDebug(&errorMessage) << "Array expected instead of" << ctx.json;
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    Container container;
    Q_FOREACH (const QJsonValue &v, ctx.json.toArray()) {
        typename Container::value_type item;
        if (!deserialize(v, item, ctx.errorInfo)) {
            return false;
        }
        container.push_back(item);
    }
    ctx.data.swap(container);
    return true;
}

template<Action action, typename T, typename Allocator>
bool mapValue(const Context<action, std::list<T, Allocator> > &ctx)
{
    return mapSequence(ctx);
}

template<Action action, typename T, typename Allocator>
bool mapValue(const Context<action, std::vector<T, Allocator> > &ctx)
{
    return mapSequence(ctx);
}

template<Action action, typename T>
bool mapValue(const Context<action, QList<T> > &ctx)
{
    return mapSequence(ctx);
}

template<Action action, typename T>
bool mapValue(const Context<action, QVector<T> > &ctx)
{
    return mapSequence(ctx);
}

template<Action action>
bool mapValue(const Context<action, QStringList> &ctx)
{
    return mapSequence(ctx);
}

template<typename Pair>
bool mapPair(const Context<Serialize, Pair> &ctx)
{
    QJsonArray array;
    QJsonValue first, second;
    if (!serialize(first, ctx.data.first, ctx.errorInfo) ||
            !serialize(second, ctx.data.second, ctx.errorInfo))
    {
        return false;
    }
    array.push_back(first);
    array.push_back(second);
    ctx.json = array;
    return true;
}

template<typename Pair>
bool mapPair(const Context<Deserialize, Pair> &ctx)
{
    if (!ctx.json.isArray()) {
        QString errorMessage;
        QDebug(&errorMessage) << "Array expected instead of" << ctx.json;
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    QJsonArray array(ctx.json.toArray());
    if (array.size() != 2) {
        QString errorMessage;
        QDebug(&errorMessage) << "Expected array of size 2, actual size is" << array.size();
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    Pair newData;
    if (!deserialize(array[0], newData.first, ctx.errorInfo) ||
            !deserialize(array[1], newData.second, ctx.errorInfo))
    {
        return false;
    }
    ctx.data = newData;
    return true;
}

template<Action action, typename First, typename Second>
bool mapValue(const Context<action, std::pair<First, Second> > &ctx)
{
    return mapPair(ctx);
}

template<Action action, typename First, typename Second>
bool mapValue(const Context<action, QPair<First, Second> > &ctx)
{
    return mapPair(ctx);
}

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
bool mapAssociative(const Context<Serialize, typename Traits::Container> &ctx)
{
    QJsonObject object;
    for (typename Traits::Iterator i = ctx.data.begin(); i != ctx.data.end(); ++i) {
        QJsonValue keyJson;
        if (!serialize(keyJson, Traits::key(i), ctx.errorInfo)) {
            return false;
        }
        if (!keyJson.isString()) {
            QString errorMessage;
            QDebug(&errorMessage) << "String expected instead of" << keyJson;
            ctx.errorInfo.append(errorMessage);
            return false;
        }
        QString keyString(keyJson.toString());
        if (object.contains(keyString)) {
            ctx.errorInfo.append(QStringLiteral("Duplicate key '%1'").arg(keyString));
            return false;
        }
        QJsonValue valueJson;
        if (!serialize(valueJson, Traits::value(i), ctx.errorInfo)) {
            ctx.errorInfo.append(QStringLiteral("Can't generate JSON for value of '%1'").arg(keyString));
            return false;
        }
        object.insert(keyString, valueJson);
    }
    ctx.json = object;
    return true;
}

template<typename Traits>
bool mapAssociative(const Context<Deserialize, typename Traits::Container> &ctx)
{
    if (!ctx.json.isObject()) {
        return false;
    }
    typename Traits::Container newData;
    QJsonObject object(ctx.json.toObject());
    for (QJsonObject::ConstIterator i = object.begin(); i != object.end(); ++i) {
        typename Traits::KeyType key;
        if (!deserialize(QJsonValue(i.key()), key, ctx.errorInfo)) {
            return false;
        }
        if (newData.count(key)) {
            ctx.errorInfo.append(QStringLiteral("Duplicate key '%1'").arg(i.key()));
            return false;
        }
        typename Traits::ValueType value;
        if (!deserialize(i.value(), value, ctx.errorInfo)) {
            ctx.errorInfo.append(QStringLiteral("Can't parse value for key '%1'").arg(i.key()));
            return false;
        }
        Traits::insert(newData, key, value);
    }
    ctx.data.swap(newData);
    return true;
}

template<Action action, typename Key, typename Value>
bool mapValue(const Context<action, QMap<Key, Value> > &ctx)
{
    return mapAssociative<QtMapTraits<QMap<Key, Value> > >(ctx);
}

template<Action action, typename Key, typename Value>
bool mapValue(const Context<action, QHash<Key, Value> > &ctx)
{
    return mapAssociative<QtMapTraits<QHash<Key, Value> > >(ctx);
}

template<Action action, typename Key, typename Value, typename Predicate, typename Allocator>
bool mapValue(const Context<action, std::map<Key, Value, Predicate, Allocator> > &ctx)
{
    return mapAssociative<StdMapTraits<std::map<Key, Value, Predicate, Allocator> > >(ctx);
}

namespace util
{

template<typename Container>
size_t arraySize(const Container &container)
{
    return container.size();
}

template<typename T, size_t N>
size_t arraySize(T (&)[N])
{
    return N;
}

}

template<typename T, typename StringArray>
bool mapEnum(const Context<Serialize, T> &ctx, const StringArray &values)
{
    if (static_cast<size_t>(ctx.data) >= util::arraySize(values)) {
        ctx.errorInfo.append(QStringLiteral("Enum value %1 too big, max. possible value is %2")
                             .arg(ctx.data).arg(util::arraySize(values)));
        return false;
    }
    ctx.json = QJsonValue(QString(values[ctx.data]));
    return true;
}

template<typename T, typename StringArray>
bool mapEnum(const Context<Deserialize, T> &ctx, const StringArray &values)
{
    size_t n = util::arraySize(values);
    QString stringValue;
    if (!deserialize(ctx.json, stringValue, ctx.errorInfo)) {
        return false;
    }
    for (size_t i = 0; i < n; i++) {
        if (stringValue == values[i]) {
            ctx.data = static_cast<T>(i);
            return true;
        }
    }
    ctx.errorInfo.append(QStringLiteral("Unknown enum value '%1'").arg(stringValue));
    return false;
}

template<typename T>
bool mapEnum(const Context<Serialize, T> &ctx, const QMetaEnum &metaEnum)
{
    const char *value = metaEnum.valueToKey(ctx.data);
    if (!value) {
        QString errorMessage;
        QDebug(&errorMessage) << "Unknown enum value" << ctx.data << "(" << metaEnum.name() << ")";
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    ctx.json = QJsonValue(QString::fromUtf8(value));
    return true;
}

template<typename T>
bool mapEnum(const Context<Deserialize, T> &ctx, const QMetaEnum &metaEnum)
{
    QByteArray stringValue;
    if (!deserialize(ctx.json, stringValue, ctx.errorInfo)) {
        return false;
    }
    bool ok = false;
    int intValue = metaEnum.keyToValue(stringValue.constData(), &ok);
    if (ok) {
        ctx.data = static_cast<T>(intValue);
    } else {
        QString errorMessage;
        QDebug(&errorMessage) << "Unknown enum value" << stringValue << "(" << metaEnum.name() << ")";
        ctx.errorInfo.append(errorMessage);
    }
    return ok;
}

template<Action action, typename T>
bool mapEnum(const Context<action, T> &ctx, const QMetaObject &metaObject, const char *enumName)
{
    int enumIndex = metaObject.indexOfEnumerator(enumName);
    if (enumIndex < 0) {
        QString errorMessage;
        QDebug(&errorMessage) << "Enum" << enumName << "not found in" << metaObject.className();
        ctx.errorInfo.append(errorMessage);
        return false;
    }
    return mapEnum(ctx, metaObject.enumerator(enumIndex));
}

}

#define QJSONMAPPER_MAPTOJSON_INSTANTIATE(Type) \
    namespace qjsonmapper { \
        template bool mapValue<qjsonmapper::Serialize>(const qjsonmapper::Context<qjsonmapper::Serialize, Type> &); \
        template bool mapValue<qjsonmapper::Deserialize>(const qjsonmapper::Context<qjsonmapper::Deserialize, Type> &); \
    }

#define QJSONMAPPER_MAPTOJSON_OBJECT_INSTANTIATE(Type) \
    namespace qjsonmapper { \
        template void mapObject<qjsonmapper::Serialize>(qjsonmapper::ObjectContext<qjsonmapper::Serialize, Type> &); \
        template void mapObject<qjsonmapper::Deserialize>(qjsonmapper::ObjectContext<qjsonmapper::Deserialize, Type> &); \
    }

#define QJSONMAPPER_MAPTOJSON_OBJECT_MEM_INSTANTIATE(Class) \
    template void Class::mapToJson<qjsonmapper::Serialize>(qjsonmapper::ObjectContext<qjsonmapper::Serialize, Class> &); \
    template void Class::mapToJson<qjsonmapper::Deserialize>(qjsonmapper::ObjectContext<qjsonmapper::Deserialize, Class> &);

#define QJSONMAPPER_MAPTOJSON_MEM_INSTANTIATE(Class) \
    template bool Class::mapToJson<qjsonmapper::Serialize>(const qjsonmapper::Context<qjsonmapper::Serialize, Class> &); \
    template bool Class::mapToJson<qjsonmapper::Deserialize>(const qjsonmapper::Context<qjsonmapper::Deserialize, Class> &);
