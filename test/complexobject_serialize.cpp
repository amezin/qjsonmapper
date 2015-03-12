#include "common.h"

struct SomethingComplex
{
    int a;
    std::string s;
    QString s2;

    SomethingComplex()
        : a(42), s("std::string"), s2(QStringLiteral("QString")), getSet(0)
    {
    }

    int get() const
    {
        return getSet;
    }

    void set(int v)
    {
        getSet = v;
    }

private:
    int getSet;
};

namespace qjsonserialize {

template<Action action>
bool map(const Args<action, SomethingComplex> &args)
{
    ObjectMapping<action> mapping(args.json);
    return mapping.map("a", args.data.a) &&
            mapping.map("s", args.data.s, args.data.s) &&
            mapping.map("s2", args.data.s2) &&
            mapping.mapGetSet("getSet", args.data, &SomethingComplex::get, &SomethingComplex::set);
}

}

int complexobject_serialize(int, char *[])
{
    SomethingComplex o;
    o.set(123);
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, o));
    VERIFY(json.isObject());
    QJsonObject object(json.toObject());
    VERIFY(object.value("a").toInt() == 42);
    VERIFY(object.value("s").toString() == "std::string");
    VERIFY(object.value("s2").toString() == "QString");
    VERIFY(object.value("getSet").toInt() == 123);
    return 0;
}
