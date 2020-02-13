//
// Project: GraphicsUtils
// File: LoadMesh.hpp
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_LOADMESH_HPP
#define GRAPHICSUTILS_LOADMESH_HPP


#include <string>


namespace gut {

    class VertexData;

    // Load mesh from .obj file to VertexData
    void loadMeshFromOBJ(const std::string& fileName, VertexData& vertexData);

} //namespace gut


#endif //GRAPHICSUTILS_LOADMESH_HPP
