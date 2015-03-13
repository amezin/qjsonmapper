#include "common.h"

#include <QObject>

class TestQObject2 : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int getSet MEMBER getSet)
    Q_PROPERTY(QString property2 MEMBER property2)
public:
    TestQObject2()
        : getSet(42), property2(QStringLiteral("property2 Value"))
    {
    }

    int getSet;
    QString property2;
};

namespace qjsonserialize {

template<Action action>
bool map(const Args<action, TestQObject2> &args)
{
    ObjectMapping<action> mapping(args.json);
    return mapping.template mapQProperty<int>("get_set", &args.data, "getSet") &&
            mapping.mapQProperty("property2", &args.data, "property2", QStringLiteral("Default Value"));
}

}

int qobject_deserialize(int, char *[])
{
    TestQObject2 o;
    QJsonObject json;
    json.insert("get_set", 123);
    VERIFY(qjsonserialize::deserialize(json, o));
    VERIFY(o.getSet == 123);
    VERIFY(o.property2 == QStringLiteral("Default Value"));
    return 0;
}

#include "qobject_deserialize.moc"
