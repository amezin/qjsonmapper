#include <QtTest>

#include "qjsonmapper.h"

class StdStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        std::string s("Hello, World!");
        QJsonValue json;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::serialize(json, s, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), QStringLiteral("Hello, World!"));
    }

    void deserialize()
    {
        std::string s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::deserialize(json, s, e));
        QCOMPARE(s, std::string("Hello, World!"));
    }
};

QTEST_MAIN(StdStringTest)

#include "stdstring.moc"
