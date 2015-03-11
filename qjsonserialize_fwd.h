/*
 * Copyright © 2015 Alexander Mezin <mezin.alexander@gmail.com>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#pragma once

class QJsonValue;

namespace qjsonserialize
{

template<typename Args>
bool map(const Args &args);

enum Action
{
    Serialize,
    Deserialize
};

template<Action action, typename Data>
class Args;

template<typename Data>
bool serialize(QJsonValue &json, const Data &data);

template<typename Data>
bool deserialize(const QJsonValue &json, Data &data);

template<Action action>
class ObjectMapping;

}
