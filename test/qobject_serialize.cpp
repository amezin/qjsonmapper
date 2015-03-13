#include "common.h"

#include <QObject>

class TestQObject1 : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int getSet MEMBER getSet)
    Q_PROPERTY(QString property2 MEMBER property2)
public:
    TestQObject1()
        : getSet(42), property2(QStringLiteral("property2 Value"))
    {
    }

    int getSet;
    QString property2;
};

namespace qjsonserialize {

template<Action action>
bool map(const Args<action, TestQObject1> &args)
{
    ObjectMapping<action> mapping(args.json);
    return mapping.template mapQProperty<int>("get_set", &args.data, "getSet") &&
            mapping.mapQProperty("property2", &args.data, "property2", QStringLiteral("Default Value"));
}

}

int qobject_serialize(int, char *[])
{
    TestQObject1 o;
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, o));
    VERIFY(json.isObject());
    VERIFY(json.toObject().value("get_set") == 42);
    VERIFY(json.toObject().value("property2") == QStringLiteral("property2 Value"))
    return 0;
}

#include "qobject_serialize.moc"
