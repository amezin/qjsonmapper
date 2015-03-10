#include "common.h"

#include <string>

#include "qjsonserialize_basic.h"

int stdstring_serialize(int, char *[])
{
    std::string s("Hello, World!");
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, s));
    VERIFY(json.isString());
    VERIFY(json.toString() == QStringLiteral("Hello, World!"));
    return 0;
}
