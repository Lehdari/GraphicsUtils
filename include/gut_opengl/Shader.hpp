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
#include <glad/glad.h>

#include <gut_utils/MathTypes.hpp>

#include "GLTypeUtils.hpp"

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

        void use() const;

        /** @brief  Set uniform variable using name
         *  @tparam T_Uniform   Uniform type
         *  @param  name        Uniform name
         *  @param  uniform     Uniform value to be set
         */
        template <typename T_Uniform>
        void setUniform(const std::string& name, const T_Uniform& uniform);

        /** @brief  Set uniform variable using uniform location(@see getUniformLocation)
         *  @tparam T_Uniform   Uniform type
         *  @param  location    Uniform location returned by getUniformLocation
         *  @param  uniform     Uniform value to be set
         *  @note
         *  @note   Faster than setting the uniform using name
         */
        template <typename T_Uniform>
        void setUniform(GLint location, const T_Uniform& uniform);

        /** @brief  Get uniform's location to be used with the location setUniform
         *  @param  name    Uniform name
         *  @return Uniform location (returns -1 if uniform does not exist)
         */
        GLint getUniformLocation(const std::string& name) const;

        // Dispatch for compute shaders
        void dispatch(GLuint nGroupsX, GLuint nGroupsY, GLuint nGroupsZ) const;

    private:
        GLuint                                      _programId;
        bool                                        _computeShader;

        struct Uniform {
            GLenum  type = GL_FALSE;
            GLint   size = 0;
            GLint   location = 0;
        };

        std::unordered_map<std::string, Uniform>    _uniforms;

        // Fill the map above
        void fetchShaderUniforms();

        // Checks name and type of an uniform and returns pointer to it
        template <typename T_Uniform>
        Uniform* getUniform(const std::string& name);
    };


    #include "Shader.inl"


} // namespace gut


#endif //GRAPHICSUTILS_SHADER_HPP
