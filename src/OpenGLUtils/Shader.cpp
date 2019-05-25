//
// Created by Lehdari on 25.5.2019.
//

#include "Shader.hpp"

#include <cstdio>
#include <memory>


Shader::Shader() :
    _programId              (0)
{ }

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
}

Shader::~Shader()
{
    glDeleteProgram(_programId);
}

void Shader::addUniform(const std::string& name)
{
    GLint pos = glGetUniformLocation(_programId, name.c_str());
    if (pos < 0) {
        fprintf(stderr, "ERROR: Could not find shader uniform with name %s\n", name.c_str());
        return;
    }
    _uniformPositions[name] = pos;
}

void Shader::setUniform(const std::string& name, float uniform) const
{
    glUniform1f(_uniformPositions.at(name), uniform);
}

void Shader::setUniform(const std::string& name, const Vec3f& uniform) const
{
    glUniform3fv(_uniformPositions.at(name), 1, uniform.data());
}

void Shader::setUniform(const std::string& name, const Vec4f& uniform) const
{
    glUniform4fv(_uniformPositions.at(name), 1, uniform.data());
}

void Shader::setUniform(const std::string& name, const Mat3f& uniform) const
{
    glUniformMatrix3fv(_uniformPositions.at(name), 1, GL_FALSE, uniform.data());
}

void Shader::setUniform(const std::string& name, const Mat4f& uniform) const
{
    glUniformMatrix4fv(_uniformPositions.at(name), 1, GL_FALSE, uniform.data());
}

void Shader::use() const
{
    glUseProgram(_programId);
}