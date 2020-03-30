//
// Project: GraphicsUtils
// File: Mesh.cpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "gut_utils/VertexData.hpp"

#include <vector>
#include <array>
#include <map>
#include <memory>
#include <cstdio>
#include <cstring>
#include <cctype>


using namespace gut;


Mesh::Mesh(void) :
    _vertexArrayObjectId    (0),
    _positionBufferId       (0),
    _normalBufferId         (0),
    _texCoordBufferId       (0),
    _elementBufferId        (0),
    _nIndices               (0),
    _usingNormals           (false),
    _usingTexCoords         (false)
{}

Mesh::Mesh(Mesh&& other) noexcept :
    _vertexArrayObjectId    (other._vertexArrayObjectId),
    _positionBufferId       (other._positionBufferId),
    _normalBufferId         (other._normalBufferId),
    _texCoordBufferId       (other._texCoordBufferId),
    _elementBufferId        (other._elementBufferId),
    _nIndices               (other._nIndices),
    _usingNormals           (other._usingNormals)
{
    other._vertexArrayObjectId = 0;
    other._positionBufferId = 0;
    other._normalBufferId = 0;
    other._texCoordBufferId = 0;
    other._elementBufferId = 0;
    other._nIndices = 0;
    other._usingNormals = false;
    other._usingTexCoords = false;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    _vertexArrayObjectId    = other._vertexArrayObjectId;
    _positionBufferId       = other._positionBufferId;
    _normalBufferId         = other._normalBufferId;
    _texCoordBufferId       = other._texCoordBufferId;
    _elementBufferId        = other._elementBufferId;
    _nIndices               = other._nIndices;
    _usingNormals           = other._usingNormals;

    other._vertexArrayObjectId = 0;
    other._positionBufferId = 0;
    other._normalBufferId = 0;
    other._texCoordBufferId = 0;
    other._elementBufferId = 0;
    other._nIndices = 0;
    other._usingNormals = false;
    other._usingTexCoords = false;

    return *this;
}

Mesh::~Mesh(void) {
    reset();
}

void Mesh::loadFromVertexData(const VertexData& vertexData)
{
    if (!vertexData.isValid()) {
        fprintf(stderr, "ERROR: Invalid VertexData\n"); // TODO logging
        return;
    }

    auto* positionContainer = vertexData.accessData("position");
    if (positionContainer == nullptr) {
        fprintf(stderr, "ERROR: No position data in VertexData\n"); // TODO logging
        return;
    }
    // Check position data type
    if (positionContainer->type != VertexData::DataType::VEC3F) {
        fprintf(stderr, "ERROR: Invalid data type for position data\n"); // TODO logging
        return;
    }

    auto* normalContainer = vertexData.accessData("normal");
    bool usingNormals = false;
    if (normalContainer != nullptr) {
        if (normalContainer->type != VertexData::DataType::VEC3F) {
            fprintf(stderr, "ERROR: Invalid data type for normal data\n"); // TODO logging
            return;
        }
        usingNormals = true;
    }

    auto* texCoordContainer = vertexData.accessData("texCoord");
    bool usingTexCoords = false;
    if (texCoordContainer != nullptr) {
        if (texCoordContainer->type != VertexData::DataType::VEC2F) {
            fprintf(stderr, "ERROR: Invalid data type for texture coordinate data\n"); // TODO logging
            return;
        }
        usingTexCoords = true;
    }

    auto& indices = vertexData.getIndices();
    auto& positions = *static_cast<Vector<Vec3f>*>(positionContainer->v);
    Vector<Vec3f>* normals = nullptr;
    if (usingNormals)
        normals = static_cast<Vector<Vec3f>*>(normalContainer->v);
    Vector<Vec2f>* texCoords = nullptr;
    if (usingTexCoords)
        texCoords = static_cast<Vector<Vec2f>*>(texCoordContainer->v);

    // release the used resources
    reset();

    _nIndices = indices.size();
    _usingNormals = usingNormals;
    _usingTexCoords = usingTexCoords;

    //  create and bind the VAO
    glGenVertexArrays(1, &_vertexArrayObjectId);
    glBindVertexArray(_vertexArrayObjectId);

    //  upload the vertex data to GPU and set up the vertex attribute arrays
    glGenBuffers(1, &_positionBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _positionBufferId);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec3f), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    if (_usingNormals) {
        glGenBuffers(1, &_normalBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBufferId);
        glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(Vec3f), normals->data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    }

    if (_usingTexCoords) {
        glGenBuffers(1, &_texCoordBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBufferId);
        glBufferData(GL_ARRAY_BUFFER, texCoords->size() * sizeof(Vec2f), texCoords->data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    }

    glGenBuffers(1, &_elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nIndices * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    //  unbind the VAO so it won't be changed outside this function
    glBindVertexArray(0);
}

void Mesh::render(
    const Shader& shader,
    const Camera& camera,
    const Mat4f& orientation) const
{
    shader.use();
    shader.setUniform("objectToWorld", orientation);
    if (_usingNormals)
        shader.setUniform("normalToWorld", Mat3f(Mat4f(orientation.inverse().transpose()).block<3,3>(0,0)));
    shader.setUniform("worldToClip", camera.worldToClip());

    glBindVertexArray(_vertexArrayObjectId);

    glDrawElements(GL_TRIANGLES, _nIndices, GL_UNSIGNED_INT, (GLvoid*)0);

    glBindVertexArray(0);
}

void Mesh::reset()
{
    if (_vertexArrayObjectId != 0)
        glDeleteVertexArrays(1, &_vertexArrayObjectId);
    if (_positionBufferId != 0)
        glDeleteBuffers(1, &_positionBufferId);
    if (_normalBufferId != 0)
        glDeleteBuffers(1, &_normalBufferId);
    if (_texCoordBufferId != 0)
        glDeleteBuffers(1, &_texCoordBufferId);
    if (_elementBufferId != 0)
        glDeleteBuffers(1, &_elementBufferId);

    _vertexArrayObjectId = 0;
    _positionBufferId = 0;
    _normalBufferId = 0;
    _texCoordBufferId = 0;
    _elementBufferId = 0;
    _nIndices = 0;
    _usingNormals = false;
    _usingTexCoords = false;
}
