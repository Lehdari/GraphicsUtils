//
// Project: GraphicsUtils
// File: GLTFLoader.hpp
//
// Copyright (c) 2022 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_GLTFLOADER_HPP
#define GRAPHICSUTILS_GLTFLOADER_HPP


#include <string>

#include <gut_utils/MathTypes.hpp>
#include <gut_utils/TypeUtils.hpp>

#include <nlohmann/json.hpp>


namespace gut {

class Node;
class Mesh;
class VertexData;


class GLTFLoader {
public:
    GLTFLoader() = default;

    // Deserialize from files
    void loadFromFile(const std::string& filename);

    // Construct objects from deserialized data and place them into containers provided
    void constructObjects(gut::Node& root, Vector<gut::Mesh>& meshes) const;

private:
    struct BufferView {
        size_t  buffer;
        size_t  byteLength;
        size_t  byteOffset;
        size_t  byteStride;
        int     target; // 34962: ARRAY_BUFFER, 34963: ELEMENT_ARRAY_BUFFER
    };

    struct Accessor {
        size_t      bufferView;
        size_t      byteOffset;
        int         componentType; // see https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#accessor-data-types
        size_t      count;
        std::string type; // see https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#accessor-data-types
    };

    struct Node {
        Vector<size_t>  children;
        size_t          mesh;
        Vec3f           translation;
    };

    struct Mesh {
        struct Primitive {
            nlohmann::json  attributes;
            int64_t         indices;
            int64_t         material;
            int             mode;
        };

        Vector<Primitive>   primitives;
    };

    struct Scene {
        Vector<size_t>  nodes;
    };

    Vector<Vector<char>>    _buffers;
    Vector<BufferView>      _bufferViews;
    Vector<Accessor>        _accessors;
    Vector<Node>            _nodes;
    Vector<Mesh>            _meshes;
    Vector<Scene>           _scenes;

    // clear internal state
    void clear();
    void createNodeChildren(gut::Node& parent, const Vector<size_t>& children) const;
    void createVertexDataFromMeshPrimitives(
        gut::VertexData& vertexData, const Vector<Mesh::Primitive>& primitives) const;

    inline static Mat4f createNodeTransfomation(const Node& node);
};


Mat4f GLTFLoader::createNodeTransfomation(const Node& node)
{
    Mat4f t;
    t<< Mat3f::Identity(),          node.translation,
        Vec3f::Zero().transpose(),  1.0f;
    return t;
}

} // namespace gut


#endif //GRAPHICSUTILS_GLTFLOADER_HPP
