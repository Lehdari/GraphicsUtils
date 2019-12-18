//
// Project: GraphicsUtils
// File: Shader.hpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#ifndef GRAPHICSUTILS_SHADER_HPP
#define GRAPHICSUTILS_SHADER_HPP


#include <string>
#include <unordered_map>
#include <GL/gl3w.h>

#include "Utils/MathTypes.hpp"

#undef near
#undef far


namespace gut {

    class Shader {
    public:
        Shader();

        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader operator=(const Shader&) = delete;
        Shader operator=(Shader&&) = delete;

        ~Shader();

        // Load shader from single shader file
        void load(const std::string& fileName, GLenum shaderType);
        // Load shader from vertex and fragment shader files
        void load(const std::string& vsFileName, const std::string& fsFileName);

        // Add new uniform (must be called before calling setUniforms);
        void addUniform(const std::string& name);

        void setUniform(const std::string& name, float uniform) const;
        void setUniform(const std::string& name, const Vec3f& uniform) const;
        void setUniform(const std::string& name, const Vec4f& uniform) const;
        void setUniform(const std::string& name, const Mat3f& uniform) const;
        void setUniform(const std::string& name, const Mat4f& uniform) const;
        void setUniform(const std::string& name, int uniform) const;

        template <std::size_t N>
        void setUniform(const std::string& name, const std::array<float, N>& uniform) const;

        void use() const;

        // Dispatch for compute shaders
        void dispatch(GLuint nGroupsX, GLuint nGroupsY, GLuint nGroupsZ) const;

    private:
        GLuint                                  _programId;
        std::unordered_map<std::string, GLint>  _uniformPositions;
        //flag indicating whether the sahder is compute shader
        bool                                    _computeShader;
    };


    template <std::size_t N>
    void Shader::setUniform(const std::string& name, const std::array<float, N>& uniform) const
    {
        glUniform1fv(_uniformPositions.at(name), N, uniform.data());
    }

} // namespace gut


#endif //GRAPHICSUTILS_SHADER_HPP
