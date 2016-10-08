#include <QtTest>

#include "qjsonmapper.h"

class IntTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        QJsonValue json(42);
        int value;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::deserialize(json, value, e));
        QCOMPARE(value, 42);
        QCOMPARE(json.toInt(-1), 42);
    }

    void serialize()
    {
        QJsonValue json;
        int value = 42;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::serialize(json, value, e));
        QCOMPARE(json.toInt(-1), 42);
        QCOMPARE(value, 42);
    }
};

QTEST_MAIN(IntTest)

#include "int.moc"
