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


#include "gut_utils/MathTypes.hpp"
#include <glad/glad.h>


namespace gut {

    class Shader;
    class Camera;
    class VertexData;


    class Mesh {
    public:
        Mesh(void);
        ~Mesh(void);

        Mesh(const Mesh& other) = delete;
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(const Mesh& other) = delete;
        Mesh& operator=(Mesh&& other) noexcept;

        // Load from VertexData
        void loadFromVertexData(const VertexData& vertexData);

        // Render the mesh
        void render(const Shader& shader,
                    const Camera& camera,
                    const Mat4f& orientation = Mat4f::Identity()) const;

    private:
        GLuint      _vertexArrayObjectId;
        GLuint      _positionBufferId;
        GLuint      _normalBufferId;
        GLuint      _texCoordBufferId;
        GLuint      _elementBufferId;

        uint64_t    _nIndices;
        bool        _usingNormals;
        bool        _usingTexCoords;

        // Function for releasing the OpenGL handles
        void reset();
    };

} // namespace gut


#endif //GRAPHICSUTILS_MESH_HPP
