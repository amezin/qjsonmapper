#include "common.h"

int qstring_serialize(int, char *[])
{
    QString s("Hello, World!");
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, s));
    VERIFY(json.isString());
    VERIFY(json.toString() == s);
    return 0;
}
