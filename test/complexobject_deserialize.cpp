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
void mapObject(ObjectMapArgs<action, SomethingComplex> &args)
{
    args.map("a", args.data.a) &&
            args.map("s", args.data.s, args.data.s) &&
            args.map("s2", args.data.s2) &&
            args.mapGetSet("getSet", args.data, &SomethingComplex::get, &SomethingComplex::set);
}

}

int complexobject_deserialize(int, char *[])
{
    SomethingComplex o;
    QJsonObject json;
    json.insert("a", 123);
    json.insert("s2", QStringLiteral("LOL"));
    json.insert("getSet", 1);
    VERIFY(qjsonserialize::deserialize(json, o));
    VERIFY(o.a == 123);
    VERIFY(o.s == "std::string");
    VERIFY(o.s2 == "LOL");
    VERIFY(o.get() == 1);
    return 0;
}
