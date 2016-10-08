#include <QtTest>

#include "qjsonmapper.h"

class StdWStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void deserialize()
    {
        std::wstring s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::deserialize(json, s, e));
        QCOMPARE(s, std::wstring(L"Hello, World!"));
    }

    void serialize()
    {
        std::wstring s(L"Hello, World!");
        QJsonValue json;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::serialize(json, s, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), QStringLiteral("Hello, World!"));
    }
};

QTEST_MAIN(StdWStringTest)

#include "stdwstring.moc"
