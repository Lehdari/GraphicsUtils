//
// Project: GraphicsUtils
// File: Shader.cpp
//
// Copyright (c) 2019 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Shader.hpp"

#include <cstdio>
#include <memory>


using namespace gut;


Shader::Shader() :
    _programId      (0),
    _computeShader  (false)
{
}

Shader::~Shader()
{
    glDeleteProgram(_programId);
}

void Shader::load(const std::string& fileName, GLenum shaderType)
{
    FILE* f = fopen(fileName.c_str(), "rb");
    if (!f)
        throw "File could not be opened!"; // TODO_EXCEPTION

    fseek(f, 0L, SEEK_END);
    auto fs = ftell(f);
    std::unique_ptr<char[]> bufUnique(new char[fs+1]);
    fseek(f, 0L, SEEK_SET);
    fread(bufUnique.get(), sizeof(char), fs, f);
    bufUnique.get()[fs] = '\0';
    fclose(f);
    const char* srcPtr = bufUnique.get();

    GLuint objectId = glCreateShader(shaderType);

    glShaderSource(objectId, 1, &srcPtr , NULL);
    glCompileShader(objectId);

    // Check compile status, throw exception in case of failure
    GLint compileStatus, infoLogLength;
    glGetShaderiv(objectId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(objectId, infoLogLength, NULL, &infoLog[0]);
        fprintf(stderr, "%s", infoLog);
        throw infoLog; // TODO_EXCEPTION: throw a proper exception
    }

    _programId = glCreateProgram();
    glAttachShader(_programId, objectId);
    glLinkProgram(_programId);

    // Check link status, throw exception in case of failure
    GLint linkStatus;
    glGetProgramiv(_programId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength];
        glGetProgramInfoLog(_programId, infoLogLength, NULL, &infoLog[0]);
        fprintf(stderr, "%s", infoLog);
        throw infoLog; // TODO_EXCEPTION: throw a proper exception
    }

    // Free the used shader object
    glDeleteShader(objectId);

    fetchShaderUniforms();

    // Set compute shader flag
    if (shaderType == GL_COMPUTE_SHADER)
        _computeShader = true;
}

void Shader::load(const std::string& vsFileName, const std::string& fsFileName)
{
    FILE* vsf = fopen(vsFileName.c_str(), "rb");
    if (!vsf)
        throw "File could not be opened!"; // TODO_EXCEPTION

    FILE* fsf = fopen(fsFileName.c_str(), "rb");
    if (!fsf)
        throw "File could not be opened!"; // TODO_EXCEPTION

    fseek(vsf, 0L, SEEK_END);
    auto vsfs = ftell(vsf);
    std::unique_ptr<char[]> vsBufUnique(new char[vsfs+1]);
    fseek(vsf, 0L, SEEK_SET);
    fread(vsBufUnique.get(), sizeof(char), vsfs, vsf);
    vsBufUnique.get()[vsfs] = '\0';
    fclose(vsf);
    const char* vsSrcPtr = vsBufUnique.get();

    fseek(fsf, 0L, SEEK_END);
    auto fsfs = ftell(fsf);
    std::unique_ptr<char[]> fsBufUnique(new char[fsfs+1]);
    fseek(fsf, 0L, SEEK_SET);
    fread(fsBufUnique.get(), sizeof(char), fsfs, fsf);
    fsBufUnique.get()[fsfs] = '\0';
    fclose(fsf);
    const char* fsSrcPtr = fsBufUnique.get();

    GLuint vsObjectId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsObjectId = glCreateShader(GL_FRAGMENT_SHADER);

    GLint compileStatus, infoLogLength;

    glShaderSource(vsObjectId, 1, &vsSrcPtr , NULL);
    glCompileShader(vsObjectId);

    glGetShaderiv(vsObjectId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        glGetShaderiv(vsObjectId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(vsObjectId, infoLogLength, NULL, &infoLog[0]);
        fprintf(stderr, "%s", infoLog);
        throw infoLog; // TODO_EXCEPTION: throw a proper exception
    }

    glShaderSource(fsObjectId, 1, &fsSrcPtr , NULL);
    glCompileShader(fsObjectId);

    glGetShaderiv(fsObjectId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        glGetShaderiv(fsObjectId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(fsObjectId, infoLogLength, NULL, &infoLog[0]);
        fprintf(stderr, "%s", infoLog);
        throw infoLog; // TODO_EXCEPTION: throw a proper exception
    }

    _programId = glCreateProgram();

    glAttachShader(_programId, fsObjectId);
    glAttachShader(_programId, vsObjectId);

    glLinkProgram(_programId);

    GLint linkStatus;
    glGetProgramiv(_programId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength];
        glGetProgramInfoLog(_programId, infoLogLength, NULL, &infoLog[0]);
        fprintf(stderr, "%s", infoLog);
        throw infoLog; // TODO_EXCEPTION: throw a proper exception
    }
    glDeleteShader(fsObjectId);
    glDeleteShader(vsObjectId);

    fetchShaderUniforms();
}

void Shader::use() const
{
    glUseProgram(_programId);
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    if (_uniforms.find(name) == _uniforms.end())
        return -1;
    return _uniforms.at(name).location;
}

void Shader::dispatch(GLuint nGroupsX, GLuint nGroupsY, GLuint nGroupsZ) const
{
    if (!_computeShader)
        return;

    glUseProgram(_programId);
    glDispatchCompute(nGroupsX, nGroupsY, nGroupsZ);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Shader::fetchShaderUniforms()
{
    GLint nUniforms;
    glGetProgramiv(_programId, GL_ACTIVE_UNIFORMS, &nUniforms);

    static GLchar name[256];
    GLsizei len;
    for (GLuint i=0; i<(GLuint)nUniforms; ++i) {
        Uniform u;
        glGetActiveUniform(_programId, i, 256, &len, &u.size, &u.type, name);
        u.location = glGetUniformLocation(_programId, name);
        _uniforms[name] = u;
    }
}


// Specializations for setUniform

// Specialization for sampler types
template <>
void Shader::setUniform<decltype(GL_TEXTURE0)>
    (const std::string& name, const decltype(GL_TEXTURE0)& uniform)
{
    if (_uniforms.find(name) == _uniforms.end()) {
        fprintf(stderr, "ERROR: Shader has no uniform named %s\n", name.c_str());
        return;
    }

    Uniform* u = &_uniforms.at(name);
    if (u->type == typeToGLEnum<GLenum>()) { // setting non-sampler uniform
        setUniform(u->location, uniform);
    }
    else { // setting sampler type
        glUniform1i(u->location, uniform);
    }
}

template <> void Shader::setUniform<float>(GLint location, const float& uniform)
{
    glUniform1f(location, uniform);
}

template <> void Shader::setUniform<double>(GLint location, const double& uniform)
{
    glUniform1d(location, uniform);
}

template <> void Shader::setUniform<int>(GLint location, const int& uniform)
{
    glUniform1i(location, uniform);
}

template <> void Shader::setUniform<unsigned int>(GLint location, const unsigned int& uniform)
{
    glUniform1ui(location, uniform);
}

template <> void Shader::setUniform<bool>(GLint location, const bool& uniform)
{
    glUniform1i(location, uniform);
}

template <> void Shader::setUniform<Vec2f>(GLint location, const Vec2f& uniform)
{
    glUniform2fv(location, 1, uniform.data());
}

template <> void Shader::setUniform<Vec3f>(GLint location, const Vec3f& uniform)
{
    glUniform3fv(location, 1, uniform.data());
}

template <> void Shader::setUniform<Vec4f>(GLint location, const Vec4f& uniform)
{
    glUniform4fv(location, 1, uniform.data());
}

template <> void Shader::setUniform<Mat2f>(GLint location, const Mat2f& uniform)
{
    glUniformMatrix2fv(location, 1, GL_FALSE, uniform.data());
}

template <> void Shader::setUniform<Mat3f>(GLint location, const Mat3f& uniform)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, uniform.data());
}

template <> void Shader::setUniform<Mat4f>(GLint location, const Mat4f& uniform)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, uniform.data());
}

template <> void Shader::setUniform<Vec2d>(GLint location, const Vec2d& uniform)
{
    glUniform2dv(location, 1, uniform.data());
}

template <> void Shader::setUniform<Vec3d>(GLint location, const Vec3d& uniform)
{
    glUniform3dv(location, 1, uniform.data());
}

template <> void Shader::setUniform<Vec4d>(GLint location, const Vec4d& uniform)
{
    glUniform4dv(location, 1, uniform.data());
}

template <> void Shader::setUniform<Mat2d>(GLint location, const Mat2d& uniform)
{
    glUniformMatrix2dv(location, 1, GL_FALSE, uniform.data());
}

template <> void Shader::setUniform<Mat3d>(GLint location, const Mat3d& uniform)
{
    glUniformMatrix3dv(location, 1, GL_FALSE, uniform.data());
}

template <> void Shader::setUniform<Mat4d>(GLint location, const Mat4d& uniform)
{
    glUniformMatrix4dv(location, 1, GL_FALSE, uniform.data());
}
