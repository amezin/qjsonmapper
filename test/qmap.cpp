#include <QtTest>

#include "qjsonserialize.h"

class QMapTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        QJsonObject object;
        object["1"] = 1.0;
        object["2"] = 2.0;
        object["3"] = 3.0;

        QMap<std::string, double> data;
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::deserialize(object, data, e));
        QCOMPARE(data.size(), 3);

        QCOMPARE(data["1"], 1.0);
        QCOMPARE(data["2"], 2.0);
        QCOMPARE(data["3"], 3.0);
    }

    void serialize()
    {
        QMap<std::string, double> data;
        data.insert("1", 1.0);
        data.insert("2", 2.0);
        data.insert("3", 3.0);

        QJsonValue json;
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::serialize(json, data, e));
        QVERIFY(json.isObject());

        QJsonObject object(json.toObject());
        QCOMPARE(object.value("1").toDouble(), 1.0);
        QCOMPARE(object.value("2").toDouble(), 2.0);
        QCOMPARE(object.value("3").toDouble(), 3.0);
    }
};

QTEST_MAIN(QMapTest)

#include "qmap.moc"
