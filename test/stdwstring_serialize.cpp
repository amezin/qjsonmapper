#include "common.h"

#include <string>

#include "qjsonserialize_basic.h"

int stdwstring_serialize(int, char *[])
{
    std::wstring s(L"Hello, World!");
    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, s));
    VERIFY(json.isString());
    VERIFY(json.toString() == QStringLiteral("Hello, World!"));
    return 0;
}
