//
// Project: GraphicsUtils
// File: Node.cpp
//
// Copyright (c) 2022 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Node.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"


using namespace gut;


Node::Node(const Mat4f& t, int64_t meshId) :
    _t      (t)
{
}

void Node::render(const Mat4f& tParent, const Vector<Mesh>& meshes, Shader& shader, const Camera& camera) const
{
    Mat4f t = tParent * _t;

    for (auto& meshId : _meshIds)
        meshes.at(meshId).render(shader, camera, t, GL_TRIANGLES); // TODO mode from mesh primitive

    for (auto& child : _children)
        child.render(t, meshes, shader, camera);
}
