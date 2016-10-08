#include "qjsonmapper_fwd.h"

#include <QMap>

class TestClass
{
    int field;
    QMap<QString, int> map;
};

namespace qjsonmapper {

template<Action action>
bool map(const Context<action, TestClass> &ctx)
{
    ObjectMapping<action> objectMapping(ctx.json);
    return objectMapping.map("field", ctx.data.field) &&
            objectMapping.map("map", ctx.data.map);
}

template<Action action>
void mapObject(ObjectContext<action, TestClass> &ctx)
{
    ctx.map("field", ctx.data.field) &&
            ctx.map("map", ctx.data.map);
}

}

int main(int, char *[])
{
    return 0;
}
