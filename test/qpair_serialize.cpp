#include "common.h"

#include <QPair>

int qpair_serialize(int, char *[])
{
    QPair<int, int> v;
    v.first = 1;
    v.second = 2;

    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, v));
    VERIFY(json.isObject());

    QJsonObject object(json.toObject());
    VERIFY(object.value("first").toInt() == 1);
    VERIFY(object.value("second").toInt() == 2);

    return 0;
}
