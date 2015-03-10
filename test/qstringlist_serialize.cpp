#include "common.h"

#include <QStringList>

#include "qjsonserialize_container.h"

int qstringlist_serialize(int, char *[])
{
    QStringList v;
    v.push_back(QStringLiteral("a"));
    v.push_back(QStringLiteral("b"));
    v.push_back(QStringLiteral("c"));

    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, v));
    VERIFY(json.isArray());

    QJsonArray array(json.toArray());
    VERIFY(array.size() == v.size());
    VERIFY(array.at(0).toString() == v[0]);
    VERIFY(array.at(1).toString() == v[1]);
    VERIFY(array.at(2).toString() == v[2]);

    return 0;
}
