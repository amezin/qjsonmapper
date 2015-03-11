#include "common.h"

#include <QPair>

int qpair_serialize(int, char *[])
{
    QPair<int, int> v;
    v.first = 1;
    v.second = 2;

    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, v));
    VERIFY(json.isArray());

    QJsonArray array(json.toArray());
    VERIFY(array.size() == 2);
    VERIFY(array[0].toInt() == 1);
    VERIFY(array[1].toInt() == 2);

    return 0;
}
