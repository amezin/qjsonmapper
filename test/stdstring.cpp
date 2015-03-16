#include <QtTest>

#include "qjsonserialize.h"

class StdStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        std::string s("Hello, World!");
        QJsonValue json;
        QVERIFY(qjsonserialize::serialize(json, s));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), QStringLiteral("Hello, World!"));
    }

    void serialize()
    {
        std::string s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        QVERIFY(qjsonserialize::deserialize(json, s));
        QCOMPARE(s, std::string("Hello, World!"));
    }
};

QTEST_MAIN(StdStringTest)

#include "stdstring.moc"
