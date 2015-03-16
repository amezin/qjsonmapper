#include <QtTest>

#include "qjsonserialize.h"

class QStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        QString s("Hello, World!");
        QJsonValue json;
        QVERIFY(qjsonserialize::serialize(json, s));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), s);
    }

    void deserialize()
    {
        QString s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        QVERIFY(qjsonserialize::deserialize(json, s));
        QCOMPARE(s, QStringLiteral("Hello, World!"));
    }
};

QTEST_MAIN(QStringTest)

#include "qstring.moc"
