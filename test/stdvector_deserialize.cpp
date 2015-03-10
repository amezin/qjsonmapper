#include "common.h"

#include <vector>

#include "qjsonserialize_container.h"

int stdvector_deserialize(int, char *[])
{
    QJsonArray array;
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);

    std::vector<int> v;
    VERIFY(qjsonserialize::deserialize(array, v));

    VERIFY(static_cast<size_t>(array.size()) == v.size());
    VERIFY(array.at(0).toInt() == v[0]);
    VERIFY(array.at(1).toInt() == v[1]);
    VERIFY(array.at(2).toInt() == v[2]);

    return 0;
}
