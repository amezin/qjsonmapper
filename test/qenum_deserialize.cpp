#include "common.h"

#include <QObject>

class TestEnum1 : public QObject
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
bool mapValue(const Context<action, TestEnum1::TestEnum> &ctx)
{
    return mapEnum(ctx, TestEnum1::staticMetaObject, "TestEnum");
}

}

int qenum_deserialize(int, char *[])
{
    QJsonValue json(QStringLiteral("Second"));
    TestEnum1::TestEnum value;
    VERIFY(qjsonserialize::deserialize(json, value));
    VERIFY(value == TestEnum1::Second);
    json = QJsonValue(QStringLiteral("Second"));
    VERIFY(qjsonserialize::deserialize(json, value));
    VERIFY(value == TestEnum1::Second);
    json = QJsonValue(QStringLiteral("First"));
    VERIFY(qjsonserialize::deserialize(json, value));
    VERIFY(value == TestEnum1::First);
    json = QJsonValue(QStringLiteral("NonExistant"));
    VERIFY(!qjsonserialize::deserialize(json, value));
    return 0;
}

#include "qenum_deserialize.moc"
