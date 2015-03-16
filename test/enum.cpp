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

    void deserialize()
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
};

QTEST_MAIN(EnumTest)

#include "enum.moc"
