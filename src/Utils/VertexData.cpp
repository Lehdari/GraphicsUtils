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
    size    (other.size),
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


VertexData::VertexData() :
    _maxIndex   (-1),
    _valid      (false)
{
}

Vector<std::string> VertexData::getDataNames() const
{
    Vector<std::string> names;

    for (auto& c : _containers)
        names.emplace_back(c.name);

    return names;
}

const VertexData::Container* VertexData::accessData(const std::string& name) const noexcept
{
    for (auto& c : _containers)
        if (c.name == name)
            return &c;

    return nullptr;
}

Vector<int64_t>& VertexData::getIndices() noexcept
{
    return _indices;
}

const Vector<int64_t>& VertexData::getIndices() const noexcept
{
    return _indices;
}

bool VertexData::validate()
{
    _maxIndex = -1;

    // Find maximum index
    for (auto& i : _indices)
        if (i > _maxIndex)
            _maxIndex = i;

    // No indices, data not valid
    if (_maxIndex < 0)
        return false;

    // Check that all data vectors are at least the size of maximum index
    for (auto& c : _containers) {
        if (c.size < _maxIndex)
            return false;
    }

    _valid = true;
    return true;
}

bool VertexData::isValid() const noexcept
{
    return _valid;
}