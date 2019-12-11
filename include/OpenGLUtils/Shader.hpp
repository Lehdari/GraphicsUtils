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

        void load(const std::string& vsFileName, const std::string& fsFileName);

        void addUniform(const std::string& name);

        void setUniform(const std::string& name, float uniform) const;
        void setUniform(const std::string& name, const Vec3f& uniform) const;
        void setUniform(const std::string& name, const Vec4f& uniform) const;
        void setUniform(const std::string& name, const Mat3f& uniform) const;
        void setUniform(const std::string& name, const Mat4f& uniform) const;

        void use() const;

    private:
        GLuint                                  _programId;
        std::unordered_map<std::string, GLint>  _uniformPositions;
    };

} // namespace gut


#endif //GRAPHICSUTILS_SHADER_HPP
