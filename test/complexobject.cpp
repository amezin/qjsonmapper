#include <QtTest>

#include "qjsonmapper.h"

struct SomethingComplex
{
    int a;
    std::string s;
    QString s2;
    float f;

    SomethingComplex()
        : a(42), s("std::string"),
          s2(QStringLiteral("QString")),
          f(0.0f),
          getSet(0),
          getSet2(QStringLiteral("Test"))
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

namespace qjsonmapper {

template<Action action>
void mapObject(ObjectContext<action, SomethingComplex> &ctx)
{
    ctx.mapField("a", ctx.data.a) &&
            ctx.mapField("s", ctx.data.s, ctx.data.s) &&
            ctx.mapField("s2", ctx.data.s2) &&
            ctx.mapGetSet("getSet", ctx.data, &SomethingComplex::get, &SomethingComplex::set) &&
            ctx.mapGetSet("getSet2", &SomethingComplex::get2, &SomethingComplex::set2) &&
            ctx.mapField("f", ctx.data.f, 0.0f);
}

}

class ComplexObjectTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        SomethingComplex o;
        o.set(123);
        QJsonValue json;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::serialize(json, o, e));
        QVERIFY(json.isObject());
        QJsonObject object(json.toObject());
        QCOMPARE(object.value("a").toInt(), 42);
        QCOMPARE(object.value("s").toString(), QStringLiteral("std::string"));
        QCOMPARE(object.value("s2").toString(), QStringLiteral("QString"));
        QCOMPARE(object.value("getSet").toInt(), 123);
        QCOMPARE(object.value("getSet2").toString(), QStringLiteral("Test"));
    }

    void deserialize()
    {
        SomethingComplex o;
        QJsonObject json;
        json.insert("a", 123);
        json.insert("s2", QStringLiteral("LOL"));
        json.insert("getSet", 1);
        json.insert("getSet2", QStringLiteral("Test"));
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::deserialize(json, o, e));
        QCOMPARE(o.a, 123);
        QCOMPARE(o.s, std::string("std::string"));
        QCOMPARE(o.s2, QStringLiteral("LOL"));
        QCOMPARE(o.get(), 1);
        QCOMPARE(o.get2(), QStringLiteral("Test"));
    }
};

QTEST_MAIN(ComplexObjectTest)

#include "complexobject.moc"
