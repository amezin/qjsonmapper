#include "common.h"

#include <map>

#include "qjsonserialize_container.h"

int stdmap_serialize(int, char *[])
{
    std::map<std::string, double> data;
    data.insert(std::make_pair(std::string("1"), 1.0));
    data.insert(std::make_pair(std::string("2"), 2.0));
    data.insert(std::make_pair(std::string("3"), 3.0));

    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, data));
    VERIFY(json.isObject());

    QJsonObject object(json.toObject());
    VERIFY(object.value("1").toDouble() == 1.0);
    VERIFY(object.value("2").toDouble() == 2.0);
    VERIFY(object.value("3").toDouble() == 3.0);

    return 0;
}
