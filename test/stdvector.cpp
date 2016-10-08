#include <QtTest>

#include "qjsonmapper.h"

class StdVectorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        std::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        QJsonValue json;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::serialize(json, v, e));
        QVERIFY(json.isArray());

        QJsonArray array(json.toArray());
        QCOMPARE(static_cast<size_t>(array.size()), v.size());
        QCOMPARE(array.at(0).toInt(), v[0]);
        QCOMPARE(array.at(1).toInt(), v[1]);
        QCOMPARE(array.at(2).toInt(), v[2]);
    }

    void deserialize()
    {
        QJsonArray array;
        array.push_back(1);
        array.push_back(2);
        array.push_back(3);

        std::vector<int> v;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::deserialize(array, v, e));

        QCOMPARE(static_cast<size_t>(array.size()), v.size());
        QCOMPARE(array.at(0).toInt(), v[0]);
        QCOMPARE(array.at(1).toInt(), v[1]);
        QCOMPARE(array.at(2).toInt(), v[2]);
    }
};

QTEST_MAIN(StdVectorTest)

#include "stdvector.moc"
