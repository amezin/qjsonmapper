#include <QtTest>

#include "qjsonmapper.h"

class QPairTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        QJsonArray array;
        array.push_back(1);
        array.push_back(2);

        QPair<int, int> v;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::deserialize(array, v, e));

        QCOMPARE(v.first, 1);
        QCOMPARE(v.second, 2);
    }

    void serialize()
    {
        QPair<int, int> v;
        v.first = 1;
        v.second = 2;

        QJsonValue json;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::serialize(json, v, e));
        QVERIFY(json.isArray());

        QJsonArray array(json.toArray());
        QCOMPARE(array.size(), 2);
        QCOMPARE(array[0].toInt(), 1);
        QCOMPARE(array[1].toInt(), 2);
    }
};

QTEST_MAIN(QPairTest)

#include "qpair.moc"
