//
// Project: GraphicsUtils
// File: VertexData.cpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENCE which is distributed
// with this source code package.
//

#include "VertexData.hpp"


using namespace gut;


VertexData::Container::Container(VertexData::Container&& other) :
    name    (other.name),
    type    (other.type),
    v       (other.v),
    deleter (other.deleter)
{
    other.name.clear();
    other.type = DataType::INVALID;
    other.v = nullptr;
    other.deleter = nullptr;
}

VertexData::Container& VertexData::Container::operator=(VertexData::Container&& other)
{
    name = other.name;
    type = other.type;
    v = other.v;
    deleter = other.deleter;

    other.name.clear();
    other.type = DataType::INVALID;
    other.v = nullptr;
    other.deleter = nullptr;

    return *this;
}
