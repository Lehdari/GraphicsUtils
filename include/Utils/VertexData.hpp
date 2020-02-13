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

        // Container struct storing the vertex data
        struct Container {
            std::string name;
            DataType    type;
            int64_t     size; // size of the vector below (to be used in non-type-aware contexts)
            void*       v; // pointer to data vector
            void        (*deleter)(void*);

            template <typename T_Data>
            Container(const std::string& name, T_Data* p); // pointer for type deduction

            Container(const Container&) = delete;
            Container(Container&&);
            Container& operator=(const Container&) = delete;
            Container& operator=(Container&&);

            ~Container();
        };

        VertexData();

        // Check if given type is valid vertex data type
        template <typename T_Data>
        constexpr static bool isValidDataType();

        // Generate data type identifier from type (returns INVALID for unsupported types)
        template <typename T_Data>
        constexpr static DataType getDataTypeId();

        // Add vertex data vector
        // Returns flag indicating whether the vector creation was successful
        template <typename T_Data>
        bool addDataVector(const std::string& name);

        // Same as above but with possibility to move existing vector
        template <typename T_Data>
        bool addDataVector(const std::string& name, Vector<T_Data>&& v);

        // Add data to existing data vector (does nothing in case the vector does not exist)
        template <typename T_Data>
        void addData(const std::string& name, const Vector<T_Data>& v);
        
        // Get names of the vertex data vectors
        Vector<std::string> getDataNames() const;

        // Access data container (return nullptr if container with such name does not exist)
        const Container* accessData(const std::string& name) const noexcept;

        // Set the indices vector
        void setIndices(const Vector<unsigned>& indices);
        void setIndices(Vector<unsigned>&& indices);

        // Access the indices vector
        const Vector<unsigned>& getIndices() const noexcept;

        // Validate the vertex data, ie. check that all data vectors are long enough
        // Returns flag indicating whether the vertex data was successfully validated
        bool validate();

        // Check whether the the vertex data has been validated
        bool isValid() const noexcept;

    private:
        Vector<Container>   _containers; // vertex data containers
        Vector<unsigned>    _indices; // mesh indices
        unsigned            _maxIndex; // maximum index value (all data vectors have to be this long for validity)
        bool                _valid; // flag indicating whether the vertexdata has been validated

        // Function for deleting the data vectors
        template <typename T_Data>
        static void vectorDeleter(void* v);
    };

    // Inline template member function declarations in VertexData.inl
    #include "VertexData.inl"


} //namespace gut


#endif //GRAPHICSUTILS_VERTEXDATA_HPP
