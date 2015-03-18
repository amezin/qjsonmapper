#include <QtTest>

#include "qjsonserialize.h"

class QObjectTest : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int getSet MEMBER getSet)
    Q_PROPERTY(QString property2 MEMBER property2)
    Q_PROPERTY(double property3 MEMBER property3)
    Q_PROPERTY(QString property4 MEMBER property4)
public:
    QObjectTest()
        : getSet(42),
          property2(QStringLiteral("property2 Value")),
          property3(3.0),
          property4(QStringLiteral("prop4"))
    {
    }

    int getSet;
    QString property2;
    double property3;
    QString property4;

private Q_SLOTS:
    void serialize();
    void deserialize();
};

namespace qjsonserialize {

template<Action action>
void mapObject(ObjectContext<action, QObjectTest> &mapping)
{
    mapping.template mapQProperty<int>("get_set", &mapping.data, "getSet") &&
            mapping.mapQProperty("property2", &mapping.data, "property2", QStringLiteral("Default Value")) &&
            mapping.template mapQProperty<double>("property3", "property3") &&
            mapping.mapQProp("property4", QStringLiteral("p4"));
}

}

void QObjectTest::serialize()
{
    QObjectTest o;
    QJsonValue json;
    qjsonserialize::ErrorInfo e;
    QVERIFY(qjsonserialize::serialize(json, o, e));
    QVERIFY(json.isObject());
    QCOMPARE(json.toObject().value("get_set").toInt(), 42);
    QCOMPARE(json.toObject().value("property2").toString(),
             QStringLiteral("property2 Value"));
    QCOMPARE(json.toObject().value("property3").toDouble(), 3.0);
    QCOMPARE(json.toObject().value("property4").toString(),
             QStringLiteral("prop4"));
}

void QObjectTest::deserialize()
{
    QObjectTest o;
    QJsonObject json;
    qjsonserialize::ErrorInfo e;
    json.insert("get_set", 123);
    json.insert("property3", 3.0);
    QVERIFY(qjsonserialize::deserialize(json, o, e));
    QCOMPARE(o.getSet, 123);
    QCOMPARE(o.property2, QStringLiteral("Default Value"));
    QCOMPARE(o.property3, 3.0);
    QCOMPARE(o.property4, QStringLiteral("p4"));
}

QTEST_MAIN(QObjectTest)

#include "qobject.moc"
