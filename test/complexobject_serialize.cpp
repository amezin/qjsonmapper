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

template<Action action>
bool map(const Args<action, SomethingComplex> &args)
{
    ObjectMapping<action> mapping(args.json);
    return mapping.map("a", args.data.a) &&
            mapping.map("s", args.data.s) &&
            mapping.map("s2", args.data.s2);
}

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
