//
// Project: GraphicsUtils
// File: LoadMesh.cpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "LoadMesh.hpp"
#include "VertexData.hpp"

#include <memory>
#include <map>
#include <array>


using namespace gut;


void gut::loadMeshFromOBJ(const std::string& fileName, VertexData& vertexData)
{
    Vector<Vec4f> positions;
    Vector<Vec3f> normals;
    Vector<unsigned> indices;

    FILE* f = fopen(fileName.c_str(), "rb");
    if (!f) {
        fprintf(stderr, "ERROR: Cannot open file %s\n", fileName.c_str()); // TODO logging
        return;
    }

    fseek(f, 0L, SEEK_END);
    auto fs = ftell(f);
    std::unique_ptr<char[]> bufUnique(new char[fs + 1]);
    char* buffer = bufUnique.get();
    fseek(f, 0L, SEEK_SET);
    fread(buffer, sizeof(char), fs, f);
    buffer[fs] = '\0';
    fclose(f);

    char lineHeader[32];
    Vector<Vec4f> tempPositions;
    Vector<Vec3f> tempNormals;
    Vector<std::array<unsigned, 9>> tempIndexArrays;

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
            Vec4f position = {0.0f, 0.0f, 0.0f, 1.0f};
            if (sscanf(buffer, "%f %f %f %f", &position[0], &position[1], &position[2], &position[3]) < 3)
                throw "VertexData: invalid file!"; // TODO_EXCEPTION
            tempPositions.push_back(std::move(position));
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            Vec3f normal = {0.0f, 0.0f, 0.0f};
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
            else {
                fprintf(stderr, "ERROR: Invalid file %s\n", fileName.c_str()); // TODO logging
                return;
            }
        }

        while (*buffer++ != 10)
            if (*buffer == 0)
                break;
    }

    std::map<std::array<unsigned, 3>, unsigned> createdVertices;

    bool usingNormals = tempNormals.size() > 0;

    for (auto& indexArray : tempIndexArrays) {
        if (usingNormals && (indexArray[6] == 0 || indexArray[7] == 0 || indexArray[8] == 0)){
            fprintf(stderr, "ERROR: Invalid index data in %s\n", fileName.c_str()); // TODO logging
            return;
        }

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

    if (!vertexData.getDataNames().empty()){
        fprintf(stderr, "ERROR: vertexData is required to be empty\n"); // TODO logging
        return;
    }

    vertexData.addDataVector<Vec4f>("position", std::move(positions));
    if (usingNormals)
        vertexData.addDataVector<Vec3f>("normal", std::move(normals));
    vertexData.setIndices(std::move(indices));

    if (!vertexData.validate())
        fprintf(stderr, "ERROR: VertexData validation failed\n"); // TODO logging
}
