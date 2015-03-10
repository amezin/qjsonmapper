#include "common.h"

#include "qjsonserialize_object.h"

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
    static bool map(typename Serializer::JsonObject &json,
                    typename Serializer::Data &data)
    {
        return mapAttribute(json, QStringLiteral("a"), data.a) &&
                mapAttribute(json, QStringLiteral("s"), data.s) &&
                mapAttribute(json, QStringLiteral("s2"), data.s2);
    }
};

}

int complexobject_deserialize(int, char *[])
{
    SomethingComplex o;
    QJsonObject json;
    json.insert("a", 123);
    json.insert("s2", QStringLiteral("LOL"));
    VERIFY(qjsonserialize::deserialize(json, o));
    VERIFY(o.a == 123);
    VERIFY(o.s == "std::string");
    VERIFY(o.s2 == "LOL");
    return 0;
}
