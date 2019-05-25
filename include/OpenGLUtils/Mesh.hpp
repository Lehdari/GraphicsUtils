//
// Created by lehdari on 25.5.2019.
//

#ifndef GRAPHICSUTILS_MESH_HPP
#define GRAPHICSUTILS_MESH_HPP


#include "Utils/MathTypes.hpp"
#include "GL/gl3w.h"

class Shader;
class Camera;

class Mesh {
public:
    Mesh(void);
    ~Mesh(void);

    Mesh(const Mesh& other)             = delete;
    Mesh(Mesh&& other);
    Mesh& operator=(const Mesh& other)  = delete;
    Mesh& operator=(Mesh&& other)       = delete;

    void loadFromObj(const std::string& fileName);
    void render(const Shader& shader,
                const Camera& camera,
                const Mat4f& orientation,
                const Vec3f& color = Vec3f(1.0f, 1.0f, 1.0f)) const;

private:
    GLuint _vertexArrayObjectId;
    unsigned _nIndices;

    GLuint _positionBufferId;
    GLuint _normalBufferId;
    GLuint _elementBufferId;

};

#endif //GRAPHICSUTILS_MESH_HPP
