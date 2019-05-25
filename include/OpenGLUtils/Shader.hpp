//
// Created by Lehdari on 25.5.2019.
//

#ifndef GRAPHICSUTILS_SHADER_HPP
#define GRAPHICSUTILS_SHADER_HPP


#include <string>
#include <unordered_map>
#include <GL/gl3w.h>

#include "Utils/MathTypes.hpp"


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


#endif //GRAPHICSUTILS_SHADER_HPP
