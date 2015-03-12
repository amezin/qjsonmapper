#include "common.h"

#include <QObject>

class TestQObject1 : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int getSet MEMBER getSet)
public:
    TestQObject1()
        : getSet(42)
    {
    }

    int getSet;
};

namespace qjsonserialize {

template<Action action>
bool map(const Args<action, TestQObject1> &args)
{
    ObjectMapping<action> mapping(args.json);
    return mapping.template mapQProperty<int>("get_set", &args.data, "getSet");
}

}

int qobject_serialize(int, char *[])
{
    TestQObject1 o;
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, o));
    VERIFY(json.isObject());
    VERIFY(json.toObject().value("get_set") == 42);
    return 0;
}

#include "qobject_serialize.moc"
