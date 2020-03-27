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
    _elementBufferId        (0),
    _nIndices               (0),
    _usingNormals           (false)
{}

Mesh::Mesh(Mesh&& other) noexcept :
    _vertexArrayObjectId    (other._vertexArrayObjectId),
    _positionBufferId       (other._positionBufferId),
    _normalBufferId         (other._normalBufferId),
    _elementBufferId        (other._elementBufferId),
    _nIndices               (other._nIndices),
    _usingNormals           (other._usingNormals)
{
    other._vertexArrayObjectId = 0;
    other._positionBufferId = 0;
    other._normalBufferId = 0;
    other._elementBufferId = 0;
    other._nIndices = 0;
    other._usingNormals = false;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    _vertexArrayObjectId    = other._vertexArrayObjectId;
    _positionBufferId       = other._positionBufferId;
    _normalBufferId         = other._normalBufferId;
    _elementBufferId        = other._elementBufferId;
    _nIndices               = other._nIndices;
    _usingNormals           = other._usingNormals;

    other._vertexArrayObjectId = 0;
    other._positionBufferId = 0;
    other._normalBufferId = 0;
    other._elementBufferId = 0;
    other._nIndices = 0;
    other._usingNormals = false;

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
    if (positionContainer->type != VertexData::DataType::VEC4F) {
        fprintf(stderr, "ERROR: Invalid data type for normal data\n"); // TODO logging
        return;
    }

    auto* normalContainer = vertexData.accessData("normal");
    bool usingNormals = false;
    if (normalContainer == nullptr) {
        usingNormals = false;
    }
    else {
        if (normalContainer->type != VertexData::DataType::VEC3F) {
            fprintf(stderr, "ERROR: Invalid data type for normal data\n"); // TODO logging
            return;
        }
        usingNormals = true;
    }

    auto& indices = vertexData.getIndices();
    auto& positions = *static_cast<Vector<Vec4f>*>(positionContainer->v);
    Vector<Vec3f>* normals = nullptr;
    if (usingNormals)
        normals = static_cast<Vector<Vec3f>*>(normalContainer->v);

    // release the used resources
    reset();

    _nIndices = indices.size();
    _usingNormals = usingNormals;

    //  create and bind the VAO
    glGenVertexArrays(1, &_vertexArrayObjectId);
    glBindVertexArray(_vertexArrayObjectId);

    //  upload the vertex data to GPU and set up the vertex attribute arrays
    glGenBuffers(1, &_positionBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _positionBufferId);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec4f), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    if (_usingNormals) {
        glGenBuffers(1, &_normalBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBufferId);
        glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(Vec3f), normals->data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    }

    glGenBuffers(1, &_elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nIndices * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    //  unbind the VAO so it won't be changed outside this function
    glBindVertexArray(0);
}

void Mesh::loadFromRawData(
    const float* positions,
    uint64_t nPositions,
    const float* normals,
    uint64_t nNormals,
    const uint32_t* indices,
    uint64_t nIndices)
{
    if (positions == nullptr || indices == nullptr;
        nPositions == 0 || nIndices == 0)
        return;

    // release the used resources
    reset();

    _nIndices = nIndices;
    _usingNormals = nNormals > 0 && normals != nullptr;

    //  create and bind the VAO
    glGenVertexArrays(1, &_vertexArrayObjectId);
    glBindVertexArray(_vertexArrayObjectId);

    //  upload the vertex data to GPU and set up the vertex attribute arrays
    glGenBuffers(1, &_positionBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _positionBufferId);
    glBufferData(GL_ARRAY_BUFFER, nPositions * sizeof(float) * 4, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    if (_usingNormals) {
        glGenBuffers(1, &_normalBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBufferId);
        glBufferData(GL_ARRAY_BUFFER, nNormals * sizeof(float) * 3, normals, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    }

    glGenBuffers(1, &_elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nIndices * sizeof(unsigned), indices, GL_STATIC_DRAW);

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
    if (_elementBufferId != 0)
        glDeleteBuffers(1, &_elementBufferId);

    _vertexArrayObjectId = 0;
    _positionBufferId = 0;
    _normalBufferId = 0;
    _elementBufferId = 0;
    _nIndices = 0;
    _usingNormals = false;
}
