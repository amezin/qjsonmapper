#include "common.h"

#include <QPair>

int qpair_deserialize(int, char *[])
{
    QJsonObject object;
    object["first"] = 1;
    object["second"] = 2;

    QPair<int, int> v;
    VERIFY(qjsonserialize::deserialize(object, v));

    VERIFY(v.first == 1);
    VERIFY(v.second == 2);

    return 0;
}
