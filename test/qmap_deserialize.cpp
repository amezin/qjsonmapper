#include "common.h"

#include <QMap>

#include "qjsonserialize_container.h"

int qmap_deserialize(int, char *[])
{
    QJsonObject object;
    object["1"] = 1.0;
    object["2"] = 2.0;
    object["3"] = 3.0;

    QMap<QString, double> data;
    VERIFY(qjsonserialize::deserialize(object, data));
    VERIFY(data.size() == 3);

    VERIFY(data["1"] == 1.0);
    VERIFY(data["2"] == 2.0);
    VERIFY(data["3"] == 3.0);

    return 0;
}
