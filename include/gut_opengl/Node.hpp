//
// Project: GraphicsUtils
// File: Node.hpp
//
// Copyright (c) 2022 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_NODE_HPP
#define GRAPHICSUTILS_NODE_HPP


#include <gut_utils/TypeUtils.hpp>
#include <gut_utils/MathTypes.hpp>


namespace gut {

class Mesh;
class Shader;
class Camera;


// Rendering graph node
class Node {
public:
    Node(const Mat4f& t, int64_t meshId = -1);

    // TODO refactor shader into material, and material into mesh primitive
    void render(const Mat4f& tParent, const Vector<Mesh>& meshes, Shader& shader, const Camera& camera) const;

    friend class GLTFLoader;

private:
    Mat4f           _t; // transformation relative to parent node
    Vector<size_t>  _meshIds;
    Vector<Node>    _children;
};

} // namespace gut


#endif //GRAPHICSUTILS_NODE_HPP
