/**
 * @file shader.h
 * @brief OpenGL shader program management with uniform handling and texture binding.
 *
 * @author Aaron Sharkey
 * @date 2025-07-15
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "texture.h"

/**
 * @class Shader
 * @brief Loads, compiles, and manages OpenGL shader programs with uniform/texture support.
 */
class Shader {
public:
    /**
     * @brief Load and compile vertex/fragment shaders from files.
     * @param vertexPath Path to vertex shader source
     * @param fragmentPath Path to fragment shader source
     */
    Shader(const char *vertexPath, const char *fragmentPath);

    /**
     * @brief Activate shader program for rendering.
     */
    void use() {
        glUseProgram(shaderProgram);
        resetTextureUnit();
    }

    /**
     * @brief Bind texture to next available unit and set uniform.
     * @param uniformName Sampler uniform name in shader
     * @param texture Texture to bind
     */
    void bindTexture(const std::string &uniformName, const Texture &texture) {
        texture.bind(nextTextureUnit);
        setInt(uniformName.c_str(), nextTextureUnit);
        nextTextureUnit++;
    }

    /**
     * @brief Reset texture unit counter to 0.
     */
    void resetTextureUnit() {
        nextTextureUnit = 0;
    }

    /**
     * @brief Set integer uniform.
     */
    void setInt(const char *name, int value) const;

    /**
     * @brief Set float uniform.
     */
    void setFloat(const char *name, float value) const;

    /**
     * @brief Set vec2 uniform.
     */
    void setVec2(const char *name, float x, float y) const;

    /**
     * @brief Set vec3 uniform.
     */
    void setVec3(const char *name, float x, float y, float z) const;

    /**
     * @brief Set vec4 uniform.
     */
    void setVec4(const char *name, float x, float y, float z, float w) const;

    /**
     * @brief Set mat4 uniform.
     * @param transpose Whether to transpose matrix (usually false)
     */
    void setMat4(const char *name, const glm::mat4 &matrix, GLboolean transpose = GL_FALSE) const;

    /**
     * @brief Clean up OpenGL resources.
     */
    ~Shader();

    unsigned int vertexShader = 0; ///< Vertex shader handle
    unsigned int fragmentShader = 0; ///< Fragment shader handle
    unsigned int shaderProgram = 0; ///< Linked program handle

private:
    int compileStatus{}; ///< Compile/link status
    char compilationLog[512]{}; ///< Error log buffer
    int nextTextureUnit = 0; ///< Next texture unit for binding

    /**
     * @brief Check shader compilation status and log errors.
     * @return 0 if successful, 1 if failed
     */
    int checkCompileStatus(unsigned int shader);
};

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------

inline int Shader::checkCompileStatus(const unsigned int shader) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (!compileStatus) {
        glGetShaderInfoLog(shader, 512, nullptr, compilationLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << compilationLog << std::endl;
        return 1;
    }

    return 0;
}

inline Shader::Shader(const char *vertexPath, const char *fragmentPath)
    : compileStatus(0), compilationLog{0} {
    std::string vertexCode, fragmentCode;

    std::ifstream vertexShaderFile, fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    } catch (const std::ifstream::failure &) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char *vertCode = vertexCode.c_str();
    const char *fragCode = fragmentCode.c_str();

    // Compile vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertCode, nullptr);
    glCompileShader(vertexShader);
    int vertCompStatus = checkCompileStatus(vertexShader);

    // Compile fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCode, nullptr);
    glCompileShader(fragmentShader);
    int fragCompStatus = checkCompileStatus(fragmentShader);

    // Link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int linkStatus = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, compilationLog);
        std::cerr << "ERROR::SHADER::PROGRAM_LINKING_FAILED\n" << compilationLog << std::endl;
    }

    if ((vertCompStatus + fragCompStatus + !linkStatus) > 0) {
        std::cerr << "ERROR::SHADER::FAILED_DURING_SETUP" << std::endl;
    }
}

inline void Shader::setInt(const char *name, const int value) const {
    const int uniformLocation = glGetUniformLocation(shaderProgram, name);
    if (uniformLocation == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1i(uniformLocation, value);
}

inline void Shader::setFloat(const char *name, const float value) const {
    const int uniformLocation = glGetUniformLocation(shaderProgram, name);
    if (uniformLocation == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform1f(uniformLocation, value);
}

inline void Shader::setVec2(const char *name, const float x, const float y) const {
    const int uniformLocation = glGetUniformLocation(shaderProgram, name);
    if (uniformLocation == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform2f(uniformLocation, x, y);
}

inline void Shader::setVec3(const char *name, const float x, const float y, const float z) const {
    const int uniformLocation = glGetUniformLocation(shaderProgram, name);
    if (uniformLocation == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform3f(uniformLocation, x, y, z);
}

inline void Shader::setVec4(const char *name, const float x, const float y,
                            const float z, const float w) const {
    const int uniformLocation = glGetUniformLocation(shaderProgram, name);
    if (uniformLocation == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniform4f(uniformLocation, x, y, z, w);
}

inline void Shader::setMat4(const char *name, const glm::mat4 &matrix, const GLboolean transpose) const {
    const int uniformLocation = glGetUniformLocation(shaderProgram, name);
    if (uniformLocation == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return;
    }
    glUniformMatrix4fv(uniformLocation, 1, transpose, glm::value_ptr(matrix));
}

inline Shader::~Shader() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
}
