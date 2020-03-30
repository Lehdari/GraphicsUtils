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

in vec3 pos;
in vec3 norm;
in vec2 texc;

uniform sampler2D diffuse;

out vec3 color;

const vec3 light_dir = normalize(vec3(-1, -0.5, -1));
const vec3 light_col = vec3(1);

void main() {
    // Basic diffuse shading with directional light
    vec3 to_light = -light_dir;
    float attenuation = max(dot(norm, to_light), 0);
    color = texture(diffuse, vec2(texc.x, 1-texc.y)).rgb * (0.1 + (0.9 * attenuation * light_col));
}
