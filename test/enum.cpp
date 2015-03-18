#include <QtTest>

#include "qjsonserialize.h"

enum TestEnum
{
    First,
    Second,
    Third
};

static const char* enumElements[] = {
    "First", "Second", "Third"
};

namespace qjsonserialize
{

template<Action action>
bool mapValue(const Context<action, TestEnum> &ctx)
{
    return mapEnum(ctx, enumElements);
}

}

class EnumTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
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

    void deserialize()
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
};

QTEST_MAIN(EnumTest)

#include "enum.moc"
