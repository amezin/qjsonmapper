#include <QtTest>

#include "qjsonserialize.h"

class QEnumTest : public QObject
{
    Q_OBJECT
    Q_ENUMS(TestEnum)

public:

    enum TestEnum
    {
        First,
        Second,
        Third
    };

private Q_SLOTS:

    void serialize();
    void deserialize();
};

namespace qjsonserialize
{

template<Action action>
bool mapValue(const Context<action, QEnumTest::TestEnum> &ctx)
{
    return mapEnum(ctx, QEnumTest::staticMetaObject, "TestEnum");
}

}

void QEnumTest::serialize()
{
    QJsonValue json;
    qjsonserialize::ErrorInfo e;
    QVERIFY(qjsonserialize::serialize(json, Third, e));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("Third"));
    QVERIFY(qjsonserialize::serialize(json, First, e));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("First"));
    QVERIFY(qjsonserialize::serialize(json, Second, e));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("Second"));
}

void QEnumTest::deserialize()
{
    QJsonValue json(QStringLiteral("Second"));
    TestEnum value;
    qjsonserialize::ErrorInfo e;
    QVERIFY(qjsonserialize::deserialize(json, value, e));
    QCOMPARE(value, Second);
    json = QJsonValue(QStringLiteral("Second"));
    QVERIFY(qjsonserialize::deserialize(json, value, e));
    QCOMPARE(value, Second);
    json = QJsonValue(QStringLiteral("First"));
    QVERIFY(qjsonserialize::deserialize(json, value, e));
    QCOMPARE(value, First);
    json = QJsonValue(QStringLiteral("NonExistant"));
    QVERIFY(!qjsonserialize::deserialize(json, value, e));
}

QTEST_MAIN(QEnumTest)

#include "qenum.moc"
