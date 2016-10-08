#pragma once

class QJsonValue;
class QStringList;

namespace qjsonmapper
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

#define QJSONMAPPER_MAPTOJSON_EXTERN(Type) \
    namespace qjsonmapper { \
        extern template bool mapValue<qjsonmapper::Serialize>(const qjsonmapper::Context<qjsonmapper::Serialize, Type> &); \
        extern template bool mapValue<qjsonmapper::Deserialize>(const qjsonmapper::Context<qjsonmapper::Deserialize, Type> &); \
    }

#define QJSONMAPPER_MAPTOJSON_OBJECT_EXTERN(Type) \
    namespace qjsonmapper { \
        extern template void mapObject<qjsonmapper::Serialize>(qjsonmapper::ObjectContext<qjsonmapper::Serialize, Type> &); \
        extern template void mapObject<qjsonmapper::Deserialize>(qjsonmapper::ObjectContext<qjsonmapper::Deserialize, Type> &); \
    }

#define QJSONMAPPER_MAPTOJSON_OBJECT_MEM_EXTERN(Class) \
    extern template void Class::mapToJson<qjsonmapper::Serialize>(qjsonmapper::ObjectContext<qjsonmapper::Serialize, Class> &); \
    extern template void Class::mapToJson<qjsonmapper::Deserialize>(qjsonmapper::ObjectContext<qjsonmapper::Deserialize, Class> &);

#define QJSONMAPPER_MAPTOJSON_MEM_EXTERN(Class) \
    extern template bool Class::mapToJson<qjsonmapper::Serialize>(const qjsonmapper::Context<qjsonmapper::Serialize, Class> &); \
    extern template bool Class::mapToJson<qjsonmapper::Deserialize>(const qjsonmapper::Context<qjsonmapper::Deserialize, Class> &);
