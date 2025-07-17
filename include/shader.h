/**
 * @file shader.h
 * @brief Declaration and implementation of the Shader class for OpenGL shader management.
 *        Handles loading, compiling, and linking vertex and fragment shaders into a program.
 *
 * @author Aaron Sharkey
 * @date 2025-07-15
 */

#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/**
 * @class Shader
 * @brief Loads, compiles, and links vertex and fragment shaders into a shader program.
 *        Handles OpenGL shader resource management and error reporting.
 */
class Shader {
public:
    /**
     * @brief Constructs a Shader by loading, compiling, and linking the given source files.
     *
     * @param vertexPath   Path to the vertex shader source file.
     * @param fragmentPath Path to the fragment shader source file.
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Activates the shader program for use in the current OpenGL pipeline.
     */
    void use() const { glUseProgram(shaderProgram); }

    /**
     * @brief Destructor. Cleans up OpenGL shader and program resources.
     */
    ~Shader();

    // Public for inspection/debugging
    unsigned int vertexShader = 0;     ///< OpenGL handle for vertex shader.
    unsigned int fragmentShader = 0;   ///< OpenGL handle for fragment shader.
    unsigned int shaderProgram = 0;    ///< OpenGL handle for linked shader program.

private:
    int compileStatus{};               ///< Compilation status flag.
    char compilationLog[512]{};        ///< Buffer for shader compilation log.

    /**
     * @brief Checks and logs the compile or link status of a shader or program.
     *
     * @param shader OpenGL shader or program ID.
     * @return 0 on success, 1 on failure.
     */
    int checkCompileStatus(unsigned int shader);
};

// -----------------------------------------------------------------------------
// Shader Implementation
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

inline Shader::Shader(const char* vertexPath, const char* fragmentPath)
    : compileStatus(0), compilationLog{0}
{
    std::string vertexCode, fragmentCode;

    std::ifstream vertexShaderFile, fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // --- Read shader source files into strings ---
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    } catch (const std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vertCode = vertexCode.c_str();
    const char* fragCode = fragmentCode.c_str();

    // --- Compile Vertex Shader ---
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertCode, nullptr);
    glCompileShader(vertexShader);
    int vertCompStatus = checkCompileStatus(vertexShader);

    // --- Compile Fragment Shader ---
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCode, nullptr);
    glCompileShader(fragmentShader);
    int fragCompStatus = checkCompileStatus(fragmentShader);

    // --- Link Shader Program ---
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int shaderLinkStatus = checkCompileStatus(shaderProgram);

    // --- Final Error Check ---
    if ((vertCompStatus + fragCompStatus) > 0 || shaderLinkStatus > 0) {
        std::cerr << "ERROR::SHADER::FAILED_DURING_SETUP" << std::endl;
    }
}

inline Shader::~Shader() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
}
