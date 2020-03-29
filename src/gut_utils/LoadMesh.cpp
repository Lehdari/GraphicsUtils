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


using namespace gut;


// Adapted from https://github.com/Anttifer/impromptu/blob/master/src/Mesh.cpp
void gut::loadMeshFromOBJ(const std::string& fileName, VertexData& vertexData)
{
    FILE* f = fopen(fileName.c_str(), "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Cannot open file %s\n", fileName.c_str()); // TODO logging
        return;
    }

    // Vectors to be stored into the VertexData object
    Vector<Vec3f> positions;
    Vector<Vec3f> normals;
    Vector<Vec2f> texCoords;
    Vector<unsigned> indices;

    // Temporary objects for reading
    char lineHeader[32];
    lineHeader[31] = '\0';
    bool readSuccessful = true;
    unsigned currentLine = 0;

    float data[3];
    unsigned pIndices[3];
    unsigned tIndices[3];
    unsigned nIndices[3];

    Vector<Vec3f> objPositions;
    Vector<Vec3f> objNormals;
    Vector<Vec2f> objTexCoords;

    Vector<int> positionIndices;
    Vector<int> normalIndices;
    Vector<int> texCoordIndices;

    while (fscanf(f, "%31s", lineHeader) != EOF) {
        ++currentLine;
        readSuccessful = true;

        if (!strcmp(lineHeader, "v")) {
            memset(data, 0, sizeof(data));

            if (3 == fscanf(f, "%f %f %f", &data[0], &data[1], &data[2])) {
                objPositions.emplace_back(data[0], data[1], data[2]);
                positionIndices.push_back(-1);
            }
            else
                readSuccessful = false;
        }
        else if (!strcmp(lineHeader, "vt")) {
            memset(data, 0, sizeof(data));

            if (2 == fscanf(f, "%f %f", &data[0], &data[1])) {
                objTexCoords.emplace_back(data[0], data[1]);
                texCoordIndices.push_back(-1);
            }
            else
                readSuccessful = false;
        }
        else if (!strcmp(lineHeader, "vn")) {
            memset(data, 0, sizeof(data));

            if (3 == fscanf(f, "%f %f %f", &data[0], &data[1], &data[2])) {
                objNormals.emplace_back(data[0], data[1], data[2]);
                normalIndices.push_back(-1);
            }
            else
                readSuccessful = false;
        }
        else if (!strcmp(lineHeader, "f")) {
            memset(pIndices, 0, sizeof(pIndices));
            memset(tIndices, 0, sizeof(tIndices));
            memset(nIndices, 0, sizeof(nIndices));

            if (3 == fscanf(f, "%u %u %u", &pIndices[0], &pIndices[1], &pIndices[2]))
            {
                for (int i=0; i<3; ++i) {
                    int pi = pIndices[i]-1;

                    if (positionIndices[pi] < 0) { // value of this index is yet to be used
                        positions.emplace_back(objPositions[pi]);
                        positionIndices[pi] = positions.size()-1;
                    }

                    indices.push_back((unsigned)positionIndices[pi]);
                }
            }
            else if (5 == fscanf(f, "/%u %u/%u %u/%u", &tIndices[0],
                &pIndices[1], &tIndices[1],
                &pIndices[2], &tIndices[2]))
            {
                for (int i=0; i<3; ++i) {
                    int pi = pIndices[i]-1;
                    int ti = tIndices[i]-1;

                    if (positionIndices[pi] < 0 ||
                        texCoordIndices[ti] < 0 ||
                        positionIndices[pi] != texCoordIndices[pi])
                    {
                        positions.emplace_back(objPositions[pi]);
                        positionIndices[pi] = positions.size()-1;
                        texCoords.emplace_back(objTexCoords[ti]);
                        texCoordIndices[ti] = texCoords.size()-1;
                    }

                    indices.push_back((unsigned)positionIndices[pi]);
                }
            }
            else if (7 == fscanf(f, "/%u %u/%u/%u %u/%u/%u", &nIndices[0],
                &pIndices[1], &tIndices[1], &nIndices[1],
                &pIndices[2], &tIndices[2], &nIndices[2]))
            {
                for (int i=0; i<3; ++i) {
                    int pi = pIndices[i]-1;
                    int ti = tIndices[i]-1;
                    int ni = nIndices[i]-1;

                    if (positionIndices[pi] < 0 ||
                        texCoordIndices[ti] < 0 ||
                        normalIndices[ni] < 0 ||
                        positionIndices[pi] != texCoordIndices[ti] ||
                        positionIndices[pi] != normalIndices[ni])
                    {
                        positions.emplace_back(objPositions[pi]);
                        positionIndices[pi] = positions.size()-1;
                        texCoords.emplace_back(objTexCoords[ti]);
                        texCoordIndices[ti] = texCoords.size()-1;
                        normals.emplace_back(objNormals[ni]);
                        normalIndices[ni] = normals.size()-1;
                    }

                    indices.push_back((unsigned)positionIndices[pi]);
                }
            }
            else if (3 == fscanf(f, "/%u %u//%u", &nIndices[1],
                &pIndices[2], &nIndices[2]))
            {
                for (int i=0; i<3; ++i) {
                    int pi = pIndices[i]-1;
                    int ni = nIndices[i]-1;

                    if (positionIndices[pi] < 0 ||
                        normalIndices[ni] < 0 ||
                        positionIndices[pi] != normalIndices[ni])
                    {
                        positions.emplace_back(objPositions[pi]);
                        positionIndices[pi] = positions.size()-1;
                        normals.emplace_back(objNormals[ni]);
                        normalIndices[ni] = normals.size()-1;
                    }

                    indices.push_back((unsigned)positionIndices[pi]);
                }
            }
            else
                readSuccessful = false;
        }

        if (!readSuccessful)
            break;
    }

    if (!readSuccessful) {
        fprintf(stderr, "ERROR: %s failed at line %d\n",
            fileName.c_str(), currentLine); // TODO logging
        return;
    }

    if (!vertexData.getDataNames().empty()){
        fprintf(stderr, "ERROR: vertexData is required to be empty\n"); // TODO logging
        return;
    }

    // Move the data vectors to the VertexData container
    vertexData.addDataVector<Vec3f>("position", std::move(positions));
    if (!normals.empty())
        vertexData.addDataVector<Vec3f>("normal", std::move(normals));
    if (!texCoords.empty())
        vertexData.addDataVector<Vec2f>("texCoord", std::move(texCoords));
    vertexData.setIndices(std::move(indices));

    if (!vertexData.validate())
        fprintf(stderr, "ERROR: VertexData validation failed\n"); // TODO logging
}
