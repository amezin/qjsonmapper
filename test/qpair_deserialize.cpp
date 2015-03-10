#include "common.h"

#include <QPair>

#include "qjsonserialize_container.h"

int qpair_deserialize(int, char *[])
{
    QJsonArray array;
    array.push_back(1);
    array.push_back(2);

    QPair<int, int> v;
    VERIFY(qjsonserialize::deserialize(array, v));

    VERIFY(v.first == 1);
    VERIFY(v.second == 2);

    return 0;
}
