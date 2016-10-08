#include <QtTest>

#include "qjsonmapper.h"

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

namespace qjsonmapper
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
    qjsonmapper::ErrorInfo e;
    QVERIFY(qjsonmapper::serialize(json, Third, e));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("Third"));
    QVERIFY(qjsonmapper::serialize(json, First, e));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("First"));
    QVERIFY(qjsonmapper::serialize(json, Second, e));
    QVERIFY(json.isString());
    QCOMPARE(json.toString(), QStringLiteral("Second"));
}

void QEnumTest::deserialize()
{
    QJsonValue json(QStringLiteral("Second"));
    TestEnum value;
    qjsonmapper::ErrorInfo e;
    QVERIFY(qjsonmapper::deserialize(json, value, e));
    QCOMPARE(value, Second);
    json = QJsonValue(QStringLiteral("Second"));
    QVERIFY(qjsonmapper::deserialize(json, value, e));
    QCOMPARE(value, Second);
    json = QJsonValue(QStringLiteral("First"));
    QVERIFY(qjsonmapper::deserialize(json, value, e));
    QCOMPARE(value, First);
    json = QJsonValue(QStringLiteral("NonExistant"));
    QVERIFY(!qjsonmapper::deserialize(json, value, e));
}

QTEST_MAIN(QEnumTest)

#include "qenum.moc"
