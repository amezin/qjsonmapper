#include <QtTest>

#include "qjsonserialize.h"

class IntTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        QJsonValue json(42);
        int value;
        QVERIFY(qjsonserialize::deserialize(json, value));
        QCOMPARE(value, 42);
        QCOMPARE(json.toInt(-1), 42);
    }

    void serialize()
    {
        QJsonValue json;
        int value = 42;
        QVERIFY(qjsonserialize::serialize(json, value));
        QCOMPARE(json.toInt(-1), 42);
        QCOMPARE(value, 42);
    }
};

QTEST_MAIN(IntTest)

#include "int.moc"
