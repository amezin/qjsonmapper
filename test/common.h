#pragma once

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#define VERIFY(x) \
    if (!(x)) { \
        qFatal("%s is false", #x); \
    }
