#include <QtTest>

#include "qjsonserialize.h"

class StdPairTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        QJsonArray array;
        array.push_back(1);
        array.push_back(2);

        std::pair<int, int> v;
        QVERIFY(qjsonserialize::deserialize(array, v));

        QCOMPARE(v.first, 1);
        QCOMPARE(v.second, 2);
    }

    void serialize()
    {
        std::pair<int, int> v;
        v.first = 1;
        v.second = 2;

        QJsonValue json;
        QVERIFY(qjsonserialize::serialize(json, v));
        QVERIFY(json.isArray());

        QJsonArray array(json.toArray());
        QCOMPARE(array.size(), 2);
        QCOMPARE(array[0].toInt(), 1);
        QCOMPARE(array[1].toInt(), 2);
    }
};

QTEST_MAIN(StdPairTest)

#include "stdpair.moc"
