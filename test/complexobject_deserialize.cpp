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
            mapping.map("s", args.data.s, args.data.s) &&
            mapping.map("s2", args.data.s2);
}

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
