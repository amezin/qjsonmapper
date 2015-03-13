#include "qjsonserialize_fwd.h"

#include <QMap>

class TestClass
{
    int field;
    QMap<QString, int> map;
};

namespace qjsonserialize {

template<Action action>
bool map(const Args<action, TestClass> &args)
{
    ObjectMapping<action> objectMapping(args.json);
    return objectMapping.map("field", args.data.field) &&
            objectMapping.map("map", args.data.map);
}

template<Action action>
void mapObject(ObjectMapArgs<action, TestClass> &args)
{
    args.map("field", args.data.field) &&
            args.map("map", args.data.map);
}

}

int minimal_definition_compiles(int, char *[])
{
    return 0;
}
