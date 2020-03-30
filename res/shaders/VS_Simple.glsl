//
// Project: GraphicsUtils
// File: VS_Simple.glsl
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 pos;
out vec3 norm;
out vec2 texc;

uniform mat4 objectToWorld;
uniform mat3 normalToWorld;
uniform mat4 worldToClip;

void main() {
    pos = (objectToWorld * vec4(position, 1.0)).xyz;
    norm = normalize(normalToWorld * normal);
    texc = texCoord;
    gl_Position = worldToClip * vec4(pos, 1.0);
}
