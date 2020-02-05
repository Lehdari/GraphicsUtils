//
// Project: GraphicsUtils
// File: Mesh.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_MESH_HPP
#define GRAPHICSUTILS_MESH_HPP


#include "Utils/MathTypes.hpp"
#include <glad/glad.h>


namespace gut {

    class Shader;
    class Camera;

    class Mesh {
    public:
        Mesh(void);
        ~Mesh(void);

        Mesh(const Mesh& other) = delete;
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(const Mesh& other) = delete;
        Mesh& operator=(Mesh&& other) noexcept;

        // Load mesh from obj file
        void loadFromObj(const std::string& fileName);

        // Load mesh from raw vertex data arrays
        void loadFromVertexData(
            const float* positions,
            uint64_t nPositions, // number of vertex positions (size of the positions array / 4)
            const float* normals,
            uint64_t nNormals, // number of vertex normals (size of the normals array / 4)
            const uint32_t* indices,
            uint64_t nIndices);

        // Render the mesh
        void render(const Shader& shader,
                    const Camera& camera,
                    const Mat4f& orientation = Mat4f::Identity()) const;

    private:
        GLuint      _vertexArrayObjectId;
        GLuint      _positionBufferId;
        GLuint      _normalBufferId;
        GLuint      _elementBufferId;

        uint64_t    _nIndices;
        bool        _usingNormals;

        // Function for releasing the OpenGL handles
        void reset();
    };

} // namespace gut


#endif //GRAPHICSUTILS_MESH_HPP
