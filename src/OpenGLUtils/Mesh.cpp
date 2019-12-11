//
// Created by lehdari on 25.5.2019.
//

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include <vector>
#include <array>
#include <map>
#include <memory>
#include <cstdio>
#include <cstring>
#include <cctype>


using namespace gut;


Mesh::Mesh(void) :
    _vertexArrayObjectId(0),
    _positionBufferId(0),
    _normalBufferId(0),
    _elementBufferId(0)
{}

Mesh::Mesh(Mesh&& other) :
    _vertexArrayObjectId (other._vertexArrayObjectId),
    _nIndices (other._nIndices),
    _positionBufferId (other._positionBufferId),
    _normalBufferId (other._normalBufferId),
    _elementBufferId (other._elementBufferId)

{
    other._vertexArrayObjectId = 0;
    other._nIndices = 0;
    other._positionBufferId = 0;
    other._normalBufferId = 0;
    other._elementBufferId = 0;
}

Mesh::~Mesh(void) {
    glDeleteVertexArrays(1, &_vertexArrayObjectId);
    glDeleteBuffers(1, &_positionBufferId);
    glDeleteBuffers(1, &_normalBufferId);
    glDeleteBuffers(1, &_elementBufferId);
}

void Mesh::loadFromObj(const std::string& fileName) {
    std::vector<std::array<float, 4>> positions;
    std::vector<std::array<float, 3>> normals;
    std::vector<unsigned> indices;

    FILE* f = fopen(fileName.c_str(), "rb");
    if (!f)
        throw "File could not be opened!"; // TODO_EXCEPTION

    fseek(f, 0L, SEEK_END);
    auto fs = ftell(f);
    std::unique_ptr<char[]> bufUnique(new char[fs + 1]);
    char* buffer = bufUnique.get();
    fseek(f, 0L, SEEK_SET);
    fread(buffer, sizeof(char), fs, f);
    buffer[fs] = '\0';
    fclose(f);

    char lineHeader[32];
    std::vector<std::array<float, 4>> tempPositions;
    std::vector<std::array<float, 3>> tempNormals;
    std::vector<std::array<unsigned, 9>> tempIndexArrays;

    while(*buffer) {
        if (isprint(*buffer)) {
            if (sscanf(buffer, "%s", lineHeader) == 0)
                return;
        }
        else {
            ++buffer;
            continue;
        }

        buffer += strlen(lineHeader)+1;

        if (strcmp(lineHeader, "v") == 0) {
            std::array<float, 4> position = {0.0f, 0.0f, 0.0f, 1.0f};
            if (sscanf(buffer, "%f %f %f %f", &position[0], &position[1], &position[2], &position[3]) < 3)
                throw "VertexData: invalid file!"; // TODO_EXCEPTION
            tempPositions.push_back(std::move(position));
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            std::array<float, 3> normal = {0.0f, 0.0f, 0.0f};
            if (sscanf(buffer, "%f %f %f", &normal[0], &normal[1], &normal[2]) < 3)
                throw "VertexData: invalid file!"; // TODO_EXCEPTION
            tempNormals.push_back(std::move(normal));
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::array<unsigned, 9> index = {0, 0, 0,  0, 0, 0,  0, 0, 0};
            if (sscanf(buffer, "%u %u %u", &index[0], &index[1], &index[2]) == 3 ||
                sscanf(buffer, "%u//%u %u//%u %u//%u", &index[0], &index[6], &index[1], &index[7], &index[2], &index[8]) == 6 ||
                sscanf(buffer, "%u/%u %u/%u %u/%u", &index[0], &index[3], &index[1], &index[4], &index[2], &index[5]) == 6 ||
                sscanf(buffer, "%u/%u/%u %u/%u/%u %u/%u/%u", &index[0], &index[3], &index[6], &index[1], &index[4], &index[7], &index[2], &index[5], &index[8]) == 9)
                tempIndexArrays.push_back(std::move(index));
            else
                throw "VertexData: invalid file!"; // TODO_EXCEPTION
        }

        while (*buffer++ != 10)
            if (*buffer == 0)
                break;
    }

    std::map<std::array<unsigned, 3>, unsigned> createdVertices;

    bool usingNormals = tempNormals.size() > 0;
    bool usingIndexing = true;

    for (auto& indexArray : tempIndexArrays) {
        if (usingNormals && (indexArray[6] == 0 || indexArray[7] == 0 || indexArray[8] == 0))
            throw "VertexData: invalid index data (normals)";

        std::array<unsigned, 3> v1 = { indexArray[0], indexArray[3], indexArray[6] };
        std::array<unsigned, 3> v2 = { indexArray[1], indexArray[4], indexArray[7] };
        std::array<unsigned, 3> v3 = { indexArray[2], indexArray[5], indexArray[8] };

        if (createdVertices[v1] == 0) {
            positions.push_back(tempPositions.at(indexArray[0]-1));
            if (usingNormals)
                normals.push_back(tempNormals.at(indexArray[6]-1));

            createdVertices[v1] = positions.size()-1;
        }
        indices.push_back(createdVertices[v1]);

        if (createdVertices[v2] == 0) {
            positions.push_back(tempPositions.at(indexArray[1]-1));
            if (usingNormals)
                normals.push_back(tempNormals.at(indexArray[7]-1));

            createdVertices[v2] = positions.size()-1;
        }
        indices.push_back(createdVertices[v2]);

        if (createdVertices[v3] == 0) {
            positions.push_back(tempPositions.at(indexArray[2]-1));
            if (usingNormals)
                normals.push_back(tempNormals.at(indexArray[8]-1));

            createdVertices[v3] = positions.size()-1;
        }
        indices.push_back(createdVertices[v3]);
    }

    //  create and bind the VAO
    glGenVertexArrays(1, &_vertexArrayObjectId);
    glBindVertexArray(_vertexArrayObjectId);

    //  upload the vertex data to GPU and set up the vertex attribute arrays
    glGenBuffers(1, &_positionBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _positionBufferId);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(std::array<float, 4>), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    if (usingNormals) {
        glGenBuffers(1, &_normalBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBufferId);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(std::array<float, 3>), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    }

    if (usingIndexing) {
        _nIndices = indices.size();
        glGenBuffers(1, &_elementBufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nIndices * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
    }

    //  unbind the VAO so it won't be changed outside this function
    glBindVertexArray(0);
}

void Mesh::render(
    const Shader& shader,
    const Camera& camera,
    const Mat4f& orientation,
    const Vec3f& color) const
{
    shader.use();
    shader.setUniform("objectToWorld", orientation);
    shader.setUniform("normalToWorld", Mat3f(Mat4f(orientation.inverse().transpose()).block<3,3>(0,0)));
    shader.setUniform("worldToClip", camera.getWorldToClip());
    shader.setUniform("Color", color);

    glBindVertexArray(_vertexArrayObjectId);

    glDrawElements(GL_TRIANGLES, _nIndices, GL_UNSIGNED_INT, (GLvoid*)0);

    glBindVertexArray(0);
}
