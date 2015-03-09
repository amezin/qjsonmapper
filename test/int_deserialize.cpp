#include "common.h"

int int_deserialize(int , char *[])
{
    QJsonValue json(42);
    int value;
    VERIFY(qjsonserialize::deserialize(json, value));
    VERIFY(value == 42);
    VERIFY(json.toInt(-1) == 42);
    return 0;
}
