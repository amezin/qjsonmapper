#include "common.h"

enum TestEnum
{
    First,
    Second,
    Third
};

static const char* enumElements[] = {
    "First", "Second", "Third"
};

namespace qjsonserialize
{

template<Action action>
bool mapValue(const Context<action, TestEnum> &ctx)
{
    return mapEnum(ctx, enumElements);
}

}

int enum_deserialize(int, char *[])
{
    QJsonValue json(QStringLiteral("Second"));
    TestEnum value;
    VERIFY(qjsonserialize::deserialize(json, value));
    VERIFY(value == Second);
    json = QJsonValue(QStringLiteral("Second"));
    VERIFY(qjsonserialize::deserialize(json, value));
    VERIFY(value == Second);
    json = QJsonValue(QStringLiteral("First"));
    VERIFY(qjsonserialize::deserialize(json, value));
    VERIFY(value == First);
    json = QJsonValue(QStringLiteral("NonExistant"));
    VERIFY(!qjsonserialize::deserialize(json, value));
    return 0;
}
