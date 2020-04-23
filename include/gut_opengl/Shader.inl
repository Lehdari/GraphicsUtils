//
// Project: GraphicsUtils
// File: Shader.inl
//
// Copyright (c) 2020 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

template <typename T_Uniform>
void Shader::setUniform(const std::string& name, const T_Uniform& uniform)
{
    Uniform* u = getUniform<T_Uniform>(name);
    if (u == nullptr) return;
    setUniform(u->location, uniform);
}

template <> void Shader::setUniform<decltype(GL_TEXTURE0)>
    (const std::string& name, const decltype(GL_TEXTURE0)& uniform);

template <typename T_Uniform>
void Shader::setUniform(GLint location, const T_Uniform& uniform)
{
    fprintf(stderr, "ERROR: Invalid uniform type\n");
}

template <> void Shader::setUniform<float>(GLint location, const float& uniform);
template <> void Shader::setUniform<double>(GLint location, const double& uniform);
template <> void Shader::setUniform<int>(GLint location, const int& uniform);
template <> void Shader::setUniform<unsigned int>(GLint location, const unsigned int& uniform);
template <> void Shader::setUniform<bool>(GLint location, const bool& uniform);
template <> void Shader::setUniform<Vec2f>(GLint location, const Vec2f& uniform);
template <> void Shader::setUniform<Vec3f>(GLint location, const Vec3f& uniform);
template <> void Shader::setUniform<Vec4f>(GLint location, const Vec4f& uniform);
template <> void Shader::setUniform<Mat2f>(GLint location, const Mat2f& uniform);
template <> void Shader::setUniform<Mat3f>(GLint location, const Mat3f& uniform);
template <> void Shader::setUniform<Mat4f>(GLint location, const Mat4f& uniform);
template <> void Shader::setUniform<Vec2d>(GLint location, const Vec2d& uniform);
template <> void Shader::setUniform<Vec3d>(GLint location, const Vec3d& uniform);
template <> void Shader::setUniform<Vec4d>(GLint location, const Vec4d& uniform);
template <> void Shader::setUniform<Mat2d>(GLint location, const Mat2d& uniform);
template <> void Shader::setUniform<Mat3d>(GLint location, const Mat3d& uniform);
template <> void Shader::setUniform<Mat4d>(GLint location, const Mat4d& uniform);

template <typename T_Uniform>
Shader::Uniform* Shader::getUniform(const std::string& name)
{
    if (_uniforms.find(name) == _uniforms.end()) {
        fprintf(stderr, "ERROR: Shader has no uniform named %s\n", name.c_str());
        return nullptr;
    }

    Uniform* u = &_uniforms.at(name);
    if (typeToGLEnum<T_Uniform>() != u->type) {
        fprintf(stderr, "ERROR: Uniform %s type does not match to shader\n", name.c_str());
        return nullptr;
    }

    return u;
}
