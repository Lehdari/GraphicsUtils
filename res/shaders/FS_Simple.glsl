/**

    Automaatio- ja Systeemitekniikan killan robok�siprojekti
    FS_Simple.glsl

    @version    0.1
    @author     Miika 'LehdaRi' Lehtim�ki
    @date       2015-04-18

**/


#version 330

in vec3 norm;
in vec3 col;

out vec3 color;

const vec3 light_dir = normalize(vec3(-1, -0.5, -1));
const vec3 light_col = vec3(1);

void main() {
    // Basic diffuse shading with directional light
    vec3 to_light = -light_dir;
    float attenuation = max(dot(norm, to_light), 0);
    color = attenuation * light_col * col;
}
