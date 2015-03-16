#include <QtTest>

#include "qjsonserialize.h"

class StdMapTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        std::map<std::string, double> data;
        data.insert(std::make_pair(std::string("1"), 1.0));
        data.insert(std::make_pair(std::string("2"), 2.0));
        data.insert(std::make_pair(std::string("3"), 3.0));

        QJsonValue json;
        QVERIFY(qjsonserialize::serialize(json, data));
        QVERIFY(json.isObject());

        QJsonObject object(json.toObject());
        QCOMPARE(object.value("1").toDouble(), 1.0);
        QCOMPARE(object.value("2").toDouble(), 2.0);
        QCOMPARE(object.value("3").toDouble(), 3.0);
    }

    void deserialize()
    {
        QJsonObject object;
        object["1"] = 1.0;
        object["2"] = 2.0;
        object["3"] = 3.0;

        std::map<std::string, double> data;
        QVERIFY(qjsonserialize::deserialize(object, data));
        QCOMPARE(data.size(), static_cast<size_t>(3));

        QCOMPARE(data["1"], 1.0);
        QCOMPARE(data["2"], 2.0);
        QCOMPARE(data["3"], 3.0);
    }

};

QTEST_MAIN(StdMapTest)

#include "stdmap.moc"
