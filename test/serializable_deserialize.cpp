#include "common.h"

struct MySerializable2
{
    int a;
    std::string s;
    QString s2;

    MySerializable2()
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

    const QString &get2() const
    {
        return getSet2;
    }

    void set2(const QString &v)
    {
        getSet2 = v;
    }

    template<qjsonserialize::Action action>
    static void mapToJson(qjsonserialize::ObjectMapArgs<action, MySerializable2> &args)
    {
        args.mapField("a", args.data.a) &&
                args.mapField("s", args.data.s, args.data.s) &&
                args.mapField("s2", args.data.s2) &&
                args.mapGetSet("getSet", args.data, &MySerializable2::get, &MySerializable2::set) &&
                args.mapGetSet("getSet2", &MySerializable2::get2, &MySerializable2::set2);
    }

private:
    int getSet;
    QString getSet2;
};

int serializable_deserialize(int, char *[])
{
    MySerializable2 o;
    QJsonObject json;
    json.insert("a", 123);
    json.insert("s2", QStringLiteral("LOL"));
    json.insert("getSet", 1);
    json.insert("getSet2", QStringLiteral("Test"));
    VERIFY(qjsonserialize::deserialize(json, o));
    VERIFY(o.a == 123);
    VERIFY(o.s == "std::string");
    VERIFY(o.s2 == "LOL");
    VERIFY(o.get() == 1);
    VERIFY(o.get2() == "Test");
    return 0;
}
