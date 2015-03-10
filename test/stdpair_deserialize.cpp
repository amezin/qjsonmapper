#include "common.h"

#include <utility>

int stdpair_deserialize(int, char *[])
{
    QJsonObject object;
    object["first"] = 1;
    object["second"] = 2;

    std::pair<int, int> v;
    VERIFY(qjsonserialize::deserialize(object, v));

    VERIFY(v.first == 1);
    VERIFY(v.second == 2);

    return 0;
}
