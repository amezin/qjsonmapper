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
    QVERIFY(qjsonserialize::serialize(json, Third));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("Third"));
    QVERIFY(qjsonserialize::serialize(json, First));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("First"));
    QVERIFY(qjsonserialize::serialize(json, Second));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("Second"));
}

void QEnumTest::deserialize()
{
    QJsonValue json(QStringLiteral("Second"));
    TestEnum value;
    QVERIFY(qjsonserialize::deserialize(json, value));
    QCOMPARE(value, Second);
    json = QJsonValue(QStringLiteral("Second"));
    QVERIFY(qjsonserialize::deserialize(json, value));
    QCOMPARE(value, Second);
    json = QJsonValue(QStringLiteral("First"));
    QVERIFY(qjsonserialize::deserialize(json, value));
    QCOMPARE(value, First);
    json = QJsonValue(QStringLiteral("NonExistant"));
    QVERIFY(!qjsonserialize::deserialize(json, value));
}

QTEST_MAIN(QEnumTest)

#include "qenum.moc"
