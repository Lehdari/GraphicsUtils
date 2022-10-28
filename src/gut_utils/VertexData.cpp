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


VertexData::Container::Container(const VertexData::Container& other) :
    name    (other.name),
    type    (other.type),
    size    (other.size),
    v       (nullptr),
    deleter (other.deleter),
    copier  (other.copier)
{
    // Check for corrupted object
    if (deleter == nullptr || copier == nullptr) {
        name = "";
        type = MathTypeEnum::FLOAT;
        size = 0;
        deleter = nullptr;
        copier = nullptr;
        return;
    }

    v = copier(other.v);
}

VertexData::Container::Container(VertexData::Container&& other) noexcept :
    name    (std::move(other.name)),
    type    (other.type),
    size    (other.size),
    v       (other.v),
    deleter (other.deleter),
    copier  (other.copier)
{
    other.name.clear();
    other.v = nullptr;
    other.deleter = nullptr;
    other.copier = nullptr;
}

VertexData::Container& VertexData::Container::operator=(const VertexData::Container& other)
{
    name = other.name;
    type = other.type;
    size = other.size;
    v = nullptr;
    deleter = other.deleter;
    copier = other.copier;

    // Check for corrupted object
    if (deleter == nullptr || copier == nullptr) {
        name = "";
        size = 0;
        deleter = nullptr;
        copier = nullptr;
        return *this;
    }

    v = copier(other.v);
    return *this;
}

VertexData::Container& VertexData::Container::operator=(VertexData::Container&& other) noexcept
{
    name = std::move(other.name);
    type = other.type;
    size = other.size;
    v = other.v;
    deleter = other.deleter;
    copier = other.copier;

    other.name.clear();
    other.size = 0;
    other.v = nullptr;
    other.deleter = nullptr;
    other.copier = nullptr;

    return *this;
}


VertexData::VertexData() :
    _maxIndex   (0),
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

VertexData::Container* VertexData::accessData(const std::string& name) noexcept
{
    for (auto& c : _containers)
        if (c.name == name)
            return &c;

    return nullptr;
}

void VertexData::setIndices(const Vector<unsigned>& indices)
{
    _indices = indices;
    _valid = false;
}

void VertexData::setIndices(Vector<unsigned>&& indices)
{
    _indices = indices;
    _valid = false;
}

const Vector<unsigned>& VertexData::getIndices() const noexcept
{
    return _indices;
}

bool VertexData::validate()
{
    _maxIndex = 0;

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
