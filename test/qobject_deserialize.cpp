#include "common.h"

#include <QObject>

class TestQObject2 : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int getSet MEMBER getSet)
    Q_PROPERTY(QString property2 MEMBER property2)
    Q_PROPERTY(double property3 MEMBER property3)
    Q_PROPERTY(QString property4 MEMBER property4)
public:
    TestQObject2()
        : getSet(42), property2(QStringLiteral("property2 Value")), property3(0.0)
    {
    }

    int getSet;
    QString property2;
    double property3;
    QString property4;
};

namespace qjsonserialize {

template<Action action>
void mapObject(ObjectMapArgs<action, TestQObject2> &mapping)
{
    mapping.template mapQProperty<int>("get_set", &mapping.data, "getSet") &&
            mapping.mapQProperty("property2", &mapping.data, "property2", QStringLiteral("Default Value")) &&
            mapping.template mapQProperty<double>("property3", "property3") &&
            mapping.mapQProp("property4", QStringLiteral("p4"));
}

}

int qobject_deserialize(int, char *[])
{
    TestQObject2 o;
    QJsonObject json;
    json.insert("get_set", 123);
    json.insert("property3", 3.0);
    VERIFY(qjsonserialize::deserialize(json, o));
    VERIFY(o.getSet == 123);
    VERIFY(o.property2 == "Default Value");
    VERIFY(o.property3 == 3.0);
    VERIFY(o.property4 == "p4");
    return 0;
}

#include "qobject_deserialize.moc"
