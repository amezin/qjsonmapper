#include "common.h"

#include <vector>

int qstringlist_deserialize(int, char *[])
{
    QJsonArray json;
    json.push_back(QStringLiteral("a"));
    json.push_back(QStringLiteral("b"));
    json.push_back(QStringLiteral("c"));

    QStringList v;
    VERIFY(qjsonserialize::deserialize(json, v));

    VERIFY(json.size() == v.size());
    VERIFY(json.at(0).toString() == v[0]);
    VERIFY(json.at(1).toString() == v[1]);
    VERIFY(json.at(2).toString() == v[2]);

    return 0;
}
