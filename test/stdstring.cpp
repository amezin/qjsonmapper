#include <QtTest>

#include "qjsonserialize.h"

class StdStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        std::string s("Hello, World!");
        QJsonValue json;
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::serialize(json, s, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), QStringLiteral("Hello, World!"));
    }

    void deserialize()
    {
        std::string s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::deserialize(json, s, e));
        QCOMPARE(s, std::string("Hello, World!"));
    }
};

QTEST_MAIN(StdStringTest)

#include "stdstring.moc"
