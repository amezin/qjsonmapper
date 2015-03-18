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
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::serialize(json, s, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), s);
        QString emptyString("");
        QVERIFY(qjsonserialize::serialize(json, emptyString, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), emptyString);
        emptyString = QString();
        QVERIFY(qjsonserialize::serialize(json, emptyString, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), emptyString);
    }

    void deserialize()
    {
        QString s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        qjsonserialize::ErrorInfo e;
        QVERIFY(qjsonserialize::deserialize(json, s, e));
        QCOMPARE(s, QStringLiteral("Hello, World!"));
    }
};

QTEST_MAIN(QStringTest)

#include "qstring.moc"
