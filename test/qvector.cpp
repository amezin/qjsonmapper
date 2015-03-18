#include <QtTest>

#include "qjsonserialize.h"

class QVectorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        QVector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        QJsonValue json;
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::serialize(json, v, e));
        QVERIFY(json.isArray());

        QJsonArray array(json.toArray());
        QCOMPARE(array.size(), v.size());
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

        QVector<int> v;
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::deserialize(array, v, e));

        QCOMPARE(array.size(), v.size());
        QCOMPARE(array.at(0).toInt(), v[0]);
        QCOMPARE(array.at(1).toInt(), v[1]);
        QCOMPARE(array.at(2).toInt(), v[2]);
    }
};

QTEST_MAIN(QVectorTest)

#include "qvector.moc"
