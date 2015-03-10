#include "common.h"

int qvector_serialize(int, char *[])
{
    QVector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, v));
    VERIFY(json.isArray());

    QJsonArray array(json.toArray());
    VERIFY(array.size() == v.size());
    VERIFY(array.at(0).toInt() == v[0]);
    VERIFY(array.at(1).toInt() == v[1]);
    VERIFY(array.at(2).toInt() == v[2]);

    return 0;
}
