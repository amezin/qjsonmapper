#include "common.h"

#include <utility>

int stdpair_deserialize(int, char *[])
{
    QJsonArray array;
    array.push_back(1);
    array.push_back(2);

    std::pair<int, int> v;
    VERIFY(qjsonserialize::deserialize(array, v));

    VERIFY(v.first == 1);
    VERIFY(v.second == 2);

    return 0;
}
