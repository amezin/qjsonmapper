#pragma once

#include <QDebug>

#include "qjsonserialize.h"

#define VERIFY(x) \
    if (!(x)) { \
        qFatal("%s is false", #x); \
    }
