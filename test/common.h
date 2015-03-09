#pragma once

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include "qjsonserialize.h"

#define VERIFY(x) \
    if (!(x)) { \
        qFatal(#x " is false"); \
    }
