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
#include "GL/gl3w.h"

namespace gut {

    class Shader;
    class Camera;

    class Mesh {
    public:
        Mesh(void);

        ~Mesh(void);

        Mesh(const Mesh& other) = delete;

        Mesh(Mesh&& other);

        Mesh& operator=(const Mesh& other) = delete;

        Mesh& operator=(Mesh&& other) = delete;

        void loadFromObj(const std::string& fileName);

        void render(const Shader& shader,
                    const Camera& camera,
                    const Mat4f& orientation,
                    const Vec3f& color = Vec3f(1.0f, 1.0f, 1.0f)) const;

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
