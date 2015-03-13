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
bool mapValue(const Args<action, TestEnum> &args)
{
    return mapEnum(args, enumElements);
}

}

int enum_serialize(int, char *[])
{
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, Third));
    VERIFY(json.isString());
    VERIFY(json.toString() == "Third");
    VERIFY(qjsonserialize::serialize(json, First));
    VERIFY(json.isString());
    VERIFY(json.toString() == "First");
    VERIFY(qjsonserialize::serialize(json, Second));
    VERIFY(json.isString());
    VERIFY(json.toString() == "Second");
    return 0;
}
