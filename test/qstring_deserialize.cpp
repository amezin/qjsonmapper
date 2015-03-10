#include "common.h"

int qstring_deserialize(int, char *[])
{
    QString s;
    QJsonValue json(QStringLiteral("Hello, World!"));
    VERIFY(json.isString());
    VERIFY(qjsonserialize::deserialize(json, s));
    VERIFY(s == QLatin1String("Hello, World!"));
    return 0;
}
