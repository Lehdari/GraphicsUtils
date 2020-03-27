//
// Project: GraphicsUtils
// File: RenderContext.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_RENDERCONTEXT_HPP
#define GRAPHICSUTILS_RENDERCONTEXT_HPP


#include <vector>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"


namespace gut {

    struct RenderContext {
        Camera          camera;
        Shader          shader;
        Vector<Mesh>    meshes;

        RenderContext(
            const Camera& camera    = Camera(Camera::Settings())
            ) :
            camera  (camera)
        {}
    };

} // namespace gut


#endif //GRAPHICSUTILS_RENDERCONTEXT_HPP
