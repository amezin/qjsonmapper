#include "common.h"

struct SomethingComplex1
{
    int a;
    std::string s;
    QString s2;
    float f;

    SomethingComplex1()
        : a(42), s("std::string"), s2(QStringLiteral("QString")), f(0.0f), getSet(0), getSet2(QStringLiteral("Test"))
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

    const QString &get2() const
    {
        return getSet2;
    }

    void set2(const QString &v)
    {
        getSet2 = v;
    }

private:
    int getSet;
    QString getSet2;
};

namespace qjsonserialize {

template<Action action>
void mapObject(ObjectContext<action, SomethingComplex1> &ctx)
{
    ctx.mapField("a", ctx.data.a) &&
            ctx.mapField("s", ctx.data.s, ctx.data.s) &&
            ctx.mapField("s2", ctx.data.s2) &&
            ctx.mapGetSet("getSet", ctx.data, &SomethingComplex1::get, &SomethingComplex1::set) &&
            ctx.mapGetSet("getSet2", &SomethingComplex1::get2, &SomethingComplex1::set2) &&
            ctx.mapField("f", ctx.data.f, 0.0f);
}

}

int complexobject_serialize(int, char *[])
{
    SomethingComplex1 o;
    o.set(123);
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, o));
    VERIFY(json.isObject());
    QJsonObject object(json.toObject());
    VERIFY(object.value("a").toInt() == 42);
    VERIFY(object.value("s").toString() == "std::string");
    VERIFY(object.value("s2").toString() == "QString");
    VERIFY(object.value("getSet").toInt() == 123);
    VERIFY(object.value("getSet2").toString() == "Test");
    return 0;
}
