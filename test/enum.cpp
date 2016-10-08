#include <QtTest>

#include "qjsonmapper.h"

enum TestEnum
{
    First,
    Second,
    Third
};

static const char* enumElements[] = {
    "First", "Second", "Third"
};

namespace qjsonmapper
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

    void deserialize()
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
};

QTEST_MAIN(EnumTest)

#include "enum.moc"
