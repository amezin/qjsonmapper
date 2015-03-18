#include <QtTest>

#include "qjsonserialize.h"

class StdWStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        std::wstring s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::deserialize(json, s, e));
        QCOMPARE(s, std::wstring(L"Hello, World!"));
    }

    void serialize()
    {
        std::wstring s(L"Hello, World!");
        QJsonValue json;
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::serialize(json, s, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), QStringLiteral("Hello, World!"));
    }
};

QTEST_MAIN(StdWStringTest)

#include "stdwstring.moc"
