#include "common.h"

#include <string>

int stdstring_deserialize(int, char *[])
{
    static const std::string expected("Hello, World");
    std::string s;
    QJsonValue json(QString::fromStdString(expected));
    VERIFY(qjsonserialize::deserialize(json, s));
    VERIFY(s == expected);
    return 0;
}
