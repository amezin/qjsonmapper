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
void mapObject(ObjectMapArgs<action, TestQObject1> &mapping)
{
    mapping.template mapQProperty<int>("get_set", &mapping.data, "getSet") &&
            mapping.mapQProperty("property2", &mapping.data, "property2", QStringLiteral("Default Value"));
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
