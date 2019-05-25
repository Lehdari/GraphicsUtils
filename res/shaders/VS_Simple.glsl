/**

    Automaatio- ja Systeemitekniikan killan robokäsiprojekti
    VS_Simple.glsl

    @version    0.1
    @author     Miika 'LehdaRi' Lehtimäki
    @date       2015-04-18

**/


#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 pos;
out vec3 norm;
out vec3 col;

uniform mat4 objectToWorld;
uniform mat3 normalToWorld;
uniform mat4 worldToClip;
uniform vec3 Color;

void main() {
    pos = (objectToWorld * vec4(position, 1.0)).xyz;
    norm = normalize(normalToWorld * normal);
    col = Color;
    gl_Position = worldToClip * vec4(pos, 1.0);
}
