#include "common.h"

#include <QMap>

int qmap_numerickey_serialize(int, char *[])
{
    QMap<int, double> data;
    data.insert(1, 1.0);
    data.insert(2, 2.0);
    data.insert(3, 3.0);

    QJsonValue json;
    VERIFY(qjsonserialize::serialize(json, data));
    VERIFY(json.isObject());

    QJsonObject object(json.toObject());
    VERIFY(object.value("1").toDouble() == 1.0);
    VERIFY(object.value("2").toDouble() == 2.0);
    VERIFY(object.value("3").toDouble() == 3.0);

    return 0;
}
