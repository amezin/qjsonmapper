#include "common.h"

#include <QObject>

class TestEnum2 : public QObject
{
    Q_OBJECT
    Q_ENUMS(TestEnum)

public:
    enum TestEnum
    {
        First,
        Second,
        Third
    };
};

namespace qjsonserialize
{

template<Action action>
bool mapValue(const Args<action, TestEnum2::TestEnum> &args)
{
    return mapEnum(args, TestEnum2::staticMetaObject, "TestEnum");
}

}

int qenum_serialize(int, char *[])
{
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, TestEnum2::Third));
    VERIFY(json.isString());
    VERIFY(json.toString() == "Third");
    VERIFY(qjsonserialize::serialize(json, TestEnum2::First));
    VERIFY(json.isString());
    VERIFY(json.toString() == "First");
    VERIFY(qjsonserialize::serialize(json, TestEnum2::Second));
    VERIFY(json.isString());
    VERIFY(json.toString() == "Second");
    return 0;
}

#include "qenum_serialize.moc"
