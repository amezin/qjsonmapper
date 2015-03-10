#include "common.h"

#include "qjsonserialize_basic.h"

int int_serialize(int , char *[])
{
    QJsonValue json;
    int value = 42;
    VERIFY(qjsonserialize::serialize(json, value));
    VERIFY(json.toInt(-1) == 42);
    VERIFY(value == 42);
    return 0;
}
