#include <QtTest>

#include "qjsonserialize.h"

class QStringListTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        QStringList v;
        v.push_back(QStringLiteral("a"));
        v.push_back(QStringLiteral("b"));
        v.push_back(QStringLiteral("c"));

        QJsonValue json;
        QVERIFY(qjsonserialize::serialize(json, v));
        QVERIFY(json.isArray());

        QJsonArray array(json.toArray());
        QCOMPARE(array.size(), v.size());
        QCOMPARE(array.at(0).toString(), v[0]);
        QCOMPARE(array.at(1).toString(), v[1]);
        QCOMPARE(array.at(2).toString(), v[2]);
    }

    void deserialize()
    {
        QJsonArray json;
        json.push_back(QStringLiteral("a"));
        json.push_back(QStringLiteral("b"));
        json.push_back(QStringLiteral("c"));

        QStringList v;
        QVERIFY(qjsonserialize::deserialize(json, v));

        QCOMPARE(json.size(), v.size());
        QCOMPARE(json.at(0).toString(), v[0]);
        QCOMPARE(json.at(1).toString(), v[1]);
        QCOMPARE(json.at(2).toString(), v[2]);
    }
};

QTEST_MAIN(QStringListTest)

#include "qstringlist.moc"
