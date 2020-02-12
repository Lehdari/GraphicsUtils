//
// Project: GraphicsUtils
// File: VertexData.hpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_VERTEXDATA_HPP
#define GRAPHICSUTILS_VERTEXDATA_HPP


#include <cassert>

#include "TypeUtils.hpp"


namespace gut {

    class VertexData {
    public:
        // Vertex data type identifier
        enum class DataType {
            INVALID,
            FLOAT,
            DOUBLE,
            VEC2F,
            VEC3F,
            VEC4F
        };

        struct Container {
            std::string name;
            DataType    type;
            void*       v; // pointer to data vector
            void        (*deleter)(void*);

            template <typename T_Data>
            Container(const std::string& name, T_Data* p); // pointer for type deduction

            ~Container();
        };

        // Check if given type is valid vertex data type
        template <typename T_Data>
        constexpr static bool isValidDataType();

        // Generate data type identifier from type (returns INVALID for unsupported types)
        template <typename T_Data>
        constexpr static DataType getDataTypeId();

        // Add vertex data vector
        // Returns the created vector for data insertion (nullptr in case of failure)
        template <typename T_Data>
        Vector<T_Data>* addData(const std::string& name);

    private:
        Vector<Container>   _containers; // vertex data containers
        Vector<int64_t>     _indices; // mesh indices

        // Function for deleting the data vectors
        template <typename T_Data>
        static void vectorDeleter(void* v);
    };

    // Inline template member function declarations in VertexData.inl
    #include "VertexData.inl"


} //namespace gut


#endif //GRAPHICSUTILS_VERTEXDATA_HPP
