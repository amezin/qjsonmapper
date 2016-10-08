#include <QtTest>

#include "qjsonmapper.h"

class QStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void serialize()
    {
        QString s("Hello, World!");
        QJsonValue json;
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::serialize(json, s, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), s);
        QString emptyString("");
        QVERIFY(qjsonmapper::serialize(json, emptyString, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), emptyString);
        emptyString = QString();
        QVERIFY(qjsonmapper::serialize(json, emptyString, e));
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), emptyString);
    }

    void deserialize()
    {
        QString s;
        QJsonValue json(QStringLiteral("Hello, World!"));
        qjsonmapper::ErrorInfo e;
        QVERIFY(qjsonmapper::deserialize(json, s, e));
        QCOMPARE(s, QStringLiteral("Hello, World!"));
    }
};

QTEST_MAIN(QStringTest)

#include "qstring.moc"
