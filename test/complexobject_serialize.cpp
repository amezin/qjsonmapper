#include "common.h"

struct SomethingComplex
{
    int a;
    std::string s;
    QString s2;

    SomethingComplex()
        : a(42), s("std::string"), s2(QStringLiteral("QString"))
    {
    }
};

namespace qjsonserialize {

template<typename Mode>
struct Serializer<Mode, SomethingComplex>
        : public ObjectInstantiatingSerializer<Mode, SomethingComplex>
{
    static bool map(typename Mode::JsonObject &json,
                    typename SerializerTraits<Mode, SomethingComplex>::Data &data)
    {
        return mapAttribute(json, QStringLiteral("a"), data.a) &&
                mapAttribute(json, QStringLiteral("s"), data.s) &&
                mapAttribute(json, QStringLiteral("s2"), data.s2);
    }
};

}

int complexobject_serialize(int, char *[])
{
    SomethingComplex o;
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, o));
    VERIFY(json.isObject());
    QJsonObject object(json.toObject());
    VERIFY(object.value("a").toInt() == 42);
    VERIFY(object.value("s").toString() == "std::string");
    VERIFY(object.value("s2").toString() == "QString");
    return 0;
}
