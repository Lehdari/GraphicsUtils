//
// Project: GraphicsUtils
// File: VertexData.inl
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//


// Macro for adding a valid vertex data type
#define GUT_ADD_VALID_DATA_TYPE(TYPE, ID)\
    template <>\
    constexpr bool VertexData::isValidDataType<TYPE>()\
    { return true; }\
    template <>\
    constexpr VertexData::DataType VertexData::getDataTypeId<TYPE>()\
    { return DataType::ID; }


template<typename T_Data>
inline VertexData::Container::Container(
    const std::string& name,
    T_Data* p
) :
    name    (name),
    type    (getDataTypeId<T_Data>()),
    size    (0),
    v       (new Vector<T_Data>),
    deleter (&VertexData::vectorDeleter<T_Data>)
{
}

inline VertexData::Container::~Container()
{
    if (deleter != nullptr)
        deleter(v);
}

template <typename T_Data>
constexpr bool VertexData::isValidDataType()
{
    return false;
}

template <typename T_Data>
constexpr VertexData::DataType VertexData::getDataTypeId()
{
    return DataType::INVALID;
}

template <typename T_Data>
inline bool VertexData::addDataVector(const std::string& name)
{
    // Check data type validity
    static_assert(isValidDataType<T_Data>(), "Not a valid vertex data type\n");

    // Check if container with same name exists
    for (auto& c : _containers) {
        if (c.name == name) {
            fprintf(stderr, "ERROR: Vertex data container with name %s already exists\n",
                    name.c_str());
            return false;
        }
    }

    T_Data* p = nullptr; // pointer for constructor type deduction
    _containers.emplace_back(name, p);
    return true;
}

template <typename T_Data>
inline bool VertexData::addDataVector(const std::string& name, Vector<T_Data>&& v)
{
    // Check data type validity
    static_assert(isValidDataType<T_Data>(), "Not a valid vertex data type\n");

    // Check if container with same name exists
    for (auto& c : _containers) {
        if (c.name == name) {
            fprintf(stderr, "ERROR: Vertex data container with name %s already exists\n",
                name.c_str());
            return false;
        }
    }

    T_Data* p = nullptr; // pointer for constructor type deduction
    _containers.emplace_back(name, p);
    auto& c = _containers.back();
    c.size = v.size();
    *static_cast<Vector<T_Data>*>(c.v) = std::move(v);

    return true;
}

template <typename T_Data>
inline void VertexData::addData(const std::string& name, const Vector<T_Data>& v)
{
    for (auto& c : _containers) {
        if (c.name == name) {
            // Check for correct data type
            assert(c.type == getDataTypeId<T_Data>());
            
            // Add data to back of the vector
            auto& cv = *static_cast<Vector<T_Data>*>(c.v);
            cv.insert(cv.end(), v.begin(), v.end());
            c.size = cv.size();
        }
    }
}

template <typename T_Data>
inline void VertexData::vectorDeleter(void* v)
{
    if (v != nullptr)
        delete static_cast<Vector<T_Data>*>(v);
}

// Add all valid types
GUT_ADD_VALID_DATA_TYPE(float, FLOAT)
GUT_ADD_VALID_DATA_TYPE(double, DOUBLE)
GUT_ADD_VALID_DATA_TYPE(Vec2f, VEC2F)
GUT_ADD_VALID_DATA_TYPE(Vec3f, VEC3F)
GUT_ADD_VALID_DATA_TYPE(Vec4f, VEC4F)


#undef GUT_ADD_VALID_DATA_TYPE
