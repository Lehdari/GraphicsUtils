//
// Project: GraphicsUtils
// File: RenderContext.cpp
//
// Copyright (c) 2022 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "RenderContext.hpp"

#include <gut_utils/MathTypes.hpp>


using namespace gut;


RenderContext::RenderContext(const Camera& camera) :
    camera  (camera),
    root    (Mat4f::Identity())
{}

void gut::RenderContext::render()
{
    root.render(Mat4f::Identity(), meshes, shader, camera);
}
