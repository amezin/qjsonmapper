#include "common.h"

struct MySerializable1
{
    int a;
    std::string s;
    QString s2;

    MySerializable1()
        : a(42), s("std::string"), s2(QStringLiteral("QString")), getSet(0), getSet2(QStringLiteral("Test"))
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

    template<qjsonserialize::Action action>
    static void mapToJson(qjsonserialize::ObjectMapArgs<action, MySerializable1> &args)
    {
        args.mapField("a", args.data.a) &&
                args.mapField("s", args.data.s, args.data.s) &&
                args.mapField("s2", args.data.s2) &&
                args.mapGetSet("getSet", args.data, &MySerializable1::get, &MySerializable1::set) &&
                args.mapGetSet("getSet2", &MySerializable1::get2, &MySerializable1::set2);
    }

private:
    int getSet;
    QString getSet2;
};

int serializable_serialize(int, char *[])
{
    MySerializable1 o;
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