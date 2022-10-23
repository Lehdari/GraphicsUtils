//
// Project: GraphicsUtils
// File: GLTFLoader.cpp
//
// Copyright (c) 2022 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "GLTFLoader.hpp"
#include "Node.hpp"
#include "Mesh.hpp"

#include <gut_utils/VertexData.hpp>

#include <fstream>
#include <filesystem>
#include <cstdio>
#include <iostream> // TODO temp


using namespace gut;
using Json = nlohmann::json;
using Path = std::filesystem::path;


void GLTFLoader::loadFromFile(const std::string& filename)
{
    clear();

    // File directory
    Path dir = Path(filename).parent_path();

    // Open and parse the .gltf file
    Json json;
    {
        std::ifstream f(filename);
        json = Json::parse(f);
    }

    // TODO do something with this or remove? Maybe check glTF version 2.0?
    //std::cout << "asset: " << json["asset"] << std::endl;

    // Load buffers
    printf("\n==== LOADING BUFFERS ====\n"); // TODO remove debug print
    auto& buffers = json["buffers"];
    _buffers.reserve(buffers.size());
    for (auto& buffer : buffers) {
        size_t byteLength = buffer["byteLength"].get<size_t>();
        auto fullPath = dir / buffer["uri"]; // TODO check that uri field exists, data field not supported

        // Add new buffer of size byteLength
        _buffers.emplace_back(byteLength);

        printf("Reading buffer %lu from %s, size %lu... ", _buffers.size()-1, fullPath.c_str(), byteLength); // TODO remove debug print
        FILE* f = fopen(fullPath.c_str(), "rb");
        if (!f) {
            clear();
            fclose(f);
            // TODO error (corrupt gltf package most likely)
            return;
        }

        fread(_buffers.back().data(), sizeof(char), byteLength, f);
        fclose(f);
        printf("done.\n"); // TODO remove debug print
    }
    printf("---- LOADED %lu BUFFERS ----\n", _buffers.size()); // TODO remove debug print

    // Parse buffer views
    printf("\n==== PARSING BUFFER VIEWS ====\n"); // TODO remove debug print
    auto& bufferViews = json["bufferViews"];
    _bufferViews.reserve(bufferViews.size());
    for (auto& bufferView : bufferViews) {
        // TODO check that buffer view contains buffer and byteLength fields
        _bufferViews.emplace_back(
            bufferView["buffer"].get<size_t>(),
            bufferView["byteLength"].get<size_t>(),
            bufferView.contains("byteOffset") ? bufferView["byteOffset"].get<size_t>() : 0,
            bufferView.contains("byteStride") ? bufferView["byteStride"].get<size_t>() : 0,
            bufferView.contains("target") ? bufferView["target"].get<int>() : -1
        );
    }
    printf("---- PARSED %lu BUFFER VIEWS ----\n", _bufferViews.size()); // TODO remove debug print

    // Parse accessors
    printf("\n==== PARSING ACCESSORS ====\n"); // TODO remove debug print
    auto& accessors = json["accessors"];
    _accessors.reserve(accessors.size());
    for (auto& accessor : accessors) {
        // TODO check that accessor has bufferView, byteOffset, componentType, count and type fields
        _accessors.emplace_back(
            accessor["bufferView"].get<size_t>(),
            accessor.contains("byteOffset") ? accessor["byteOffset"].get<size_t>() : 0,
            accessor["componentType"].get<int>(),
            accessor["count"].get<size_t>(),
            accessor["type"]
        );
    }
    printf("---- PARSED %lu ACCESSORS ----\n", _accessors.size()); // TODO remove debug print

    // Parse nodes
    printf("\n==== PARSING NODES ====\n"); // TODO remove debug print
    auto& nodes = json["nodes"];
    _nodes.reserve(nodes.size());
    for (auto& node : nodes) {
        // TODO handle scale / rotation
        _nodes.emplace_back(
            node.contains("children") ? node["children"].get<Vector<size_t>>() : Vector<size_t>(),
            node.contains("mesh") ? node["mesh"].get<size_t>() : -1,
            node.contains("translation") ? Vec3f(node["translation"].get<Vector<float>>().data()) :
                Vec3f(0.0, 0.0, 0.0)
        );
    }
    printf("---- PARSED %lu NODES ----\n", _nodes.size()); // TODO remove debug print

    // Parse meshes
    printf("\n==== PARSING MESHES ====\n"); // TODO remove debug print
    auto& meshes = json["meshes"];
    _meshes.reserve(meshes.size());
    for (auto& mesh : meshes) {
        _meshes.emplace_back();
        auto& m = _meshes.back();

        auto& primitives = mesh["primitives"];
        m.primitives.reserve(primitives.size());
        for (auto& primitive : primitives) {
            m.primitives.emplace_back(
                primitive["attributes"],
                primitive.contains("indices") ? primitive["indices"].get<int64_t>() : -1,
                primitive.contains("material") ? primitive["material"].get<int64_t>() : -1,
                primitive.contains("mode") ? primitive["mode"].get<int>() : 4
            );
        }
    }
    printf("---- PARSED %lu MESHES ----\n", _meshes.size()); // TODO remove debug print

    // Parse scenes
    printf("\n==== PARSING SCENES ====\n"); // TODO remove debug print
    auto& scenes = json["scenes"];
    _scenes.reserve(scenes.size());
    for (auto& scene : scenes) {
        _scenes.emplace_back(
            scene.contains("nodes") ? scene["nodes"].get<Vector<size_t>>() : Vector<size_t>()
        );
    }
    printf("---- PARSED %lu SCENES ----\n", _scenes.size()); // TODO remove debug print
}

void GLTFLoader::constructObjects(gut::Node& root, Vector<gut::Mesh>& meshes) const
{
    for (auto& scene : _scenes) { // TODO maybe implement Scene class? (scene switching etc.)
        // Flatten all scenes directly to root node for now
        createNodeChildren(root, scene.nodes);
    }

    for (auto& mesh : _meshes) {
        meshes.emplace_back();
        printf("N. of primitives: %lu\n", mesh.primitives.size());
        VertexData vertexData;
        createVertexDataFromMeshPrimitives(vertexData, mesh.primitives);
        meshes.back().loadFromVertexData(vertexData);
    }
}

void GLTFLoader::clear()
{
    _buffers.clear();
}

void GLTFLoader::createNodeChildren(gut::Node& parent, const Vector<size_t>& children) const
{
    for (auto& childId : children) {
        // construct each child node
        auto& child = _nodes[childId];
        parent._children.emplace_back(createNodeTransfomation(child), child.mesh);

        // recurse to grandchildren
        createNodeChildren(parent._children.back(), child.children);
    }
}

void GLTFLoader::createVertexDataFromMeshPrimitives(
    gut::VertexData& vertexData, const Vector<Mesh::Primitive>& primitives) const
{
    // TODO handle each primitive separately

    Vector<unsigned> indicesCombined;
    for (auto& primitive : primitives) {
        printf("  indices: %ld\n", primitive.indices); // TODO remove debug print
        if (primitive.indices >= 0) {
            // load indices, add them to indicesCombined
            auto& accessor = _accessors[primitive.indices];
            auto& bufferView = _bufferViews[accessor.bufferView];
            printf("    accessor: {%lu, %lu, %d, %lu, %s}\n", // TODO remove debug print
                accessor.bufferView, accessor.byteOffset, accessor.componentType, accessor.count, accessor.type.c_str());
            printf("    bufferView: {%lu, %lu, %lu, %lu, %d}\n", // TODO remove debug print
                bufferView.buffer, bufferView.byteLength, bufferView.byteOffset, bufferView.byteStride, bufferView.target);

            //5120 BYTE
            //5121 UNSIGNED_BYTE
            //5122 SHORT
            //5123 UNSIGNED_SHORT
            //5125 UNSIGNED_INT
            //5126 FLOAT
            assert(accessor.componentType == 5123); // TODO support other data types

            // raw unsigned short pointer to the buffer, according to bufferView
            auto* indicesBufferView = reinterpret_cast<const uint16_t*>(
                _buffers.at(bufferView.buffer).data() + bufferView.byteOffset);
            size_t accessorOffset = accessor.byteOffset / sizeof(uint16_t); // offset in elements
            for (size_t i=0; i<accessor.count; ++i) {
                indicesCombined.emplace_back(indicesBufferView[accessorOffset+i]);
            }
        }

        assert(primitive.mode == 4); // TODO support other modes than triangles
        printf("primitive.mode: %d\n", primitive.mode);

        printf("  attributes:\n");
        for (auto& attribute : primitive.attributes.items()) {
            std::cout << "    " << attribute.key() << ": " << attribute.value() << std::endl;
            if (attribute.key() == "POSITION") {
                auto& accessor = _accessors[attribute.value()];
                auto& bufferView = _bufferViews[accessor.bufferView];
                printf("      accessor: {%lu, %lu, %d, %lu, %s}\n", // TODO remove debug print
                    accessor.bufferView, accessor.byteOffset, accessor.componentType, accessor.count, accessor.type.c_str());

                assert(accessor.type == "VEC3");
                vertexData.addDataVector<Vec3f>("position");

                // raw unsigned short pointer to the buffer, according to bufferView
                auto* dataBufferView = reinterpret_cast<const Vec3f*>(
                    _buffers.at(bufferView.buffer).data() + bufferView.byteOffset);
                size_t accessorOffset = accessor.byteOffset / sizeof(Vec3f); // offset in elements
                Vector<Vec3f> data;
                data.reserve(accessor.count);
                for (size_t i=0; i<accessor.count; ++i) {
                    data.emplace_back(dataBufferView[accessorOffset+i]);
                }

                vertexData.addData<Vec3f>("position", data);
                printf("      type: %s\n", accessor.type.c_str());

            }
        }

        break; // TODO remove
    }
    vertexData.setIndices(indicesCombined);
    assert(vertexData.validate());
}
