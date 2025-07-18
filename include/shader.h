/**
 * @file shader.h
 * @brief Declaration and implementation of the Shader class for managing OpenGL
 * shader programs. Handles loading, compiling, and linking vertex and fragment
 * shaders from source files.
 *
 * @author Aaron Sharkey
 * @date 2025-07-15
 */

#pragma once

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @class Shader
 * @brief Complete OpenGL shader program management system.
 * 
 * The Shader class provides a comprehensive interface for loading, compiling,
 * linking, and managing OpenGL shader programs. It handles the complete shader
 * pipeline from source file loading to uniform variable management.
 * 
 * Key Features:
 * - Automatic shader source file loading with error handling
 * - Vertex and fragment shader compilation with detailed error reporting
 * - Shader program linking with validation
 * - Uniform variable management with type safety
 * - RAII-based resource management for automatic cleanup
 * - Public access to OpenGL handles for advanced usage
 * 
 * Supported Workflow:
 * 1. Load shader source files from disk
 * 2. Compile vertex and fragment shaders separately
 * 3. Link shaders into a complete program
 * 4. Provide uniform setting interface
 * 5. Automatic cleanup on destruction
 * 
 * @note Uses RAII principles - shaders are compiled in constructor and
 *       automatically cleaned up in destructor
 * @note All OpenGL shader objects are publicly accessible for debugging
 * 
 * @see OpenGL Shading Language Specification
 * @see glCreateShader, glCompileShader, glLinkProgram
 */
class Shader {
public:
  /**
   * @brief Constructor that creates a complete shader program from source files.
   * 
   * Performs the complete shader compilation and linking process:
   * 1. Loads vertex and fragment shader source code from specified file paths
   * 2. Compiles both shaders with error checking and reporting
   * 3. Creates a shader program and links the compiled shaders
   * 4. Validates the linking process with detailed error reporting
   * 5. Reports any failures through std::cerr
   * 
   * @param vertexPath   File path to vertex shader source (.vert file)
   * @param fragmentPath File path to fragment shader source (.frag file)
   * 
   * @pre Both file paths must point to valid, readable shader source files
   * @pre OpenGL context must be current and valid
   * 
   * @throws std::ifstream::failure if shader files cannot be read
   * 
   * @post If successful, shaderProgram contains a valid linked OpenGL program
   * @post Individual shader objects remain accessible for inspection
   * @post Program is ready for use with glUseProgram
   * 
   * @note Compilation/linking errors are logged to std::cerr but do not throw
   * @note Failed shaders may still create objects - check compilation status
   * 
   * @see glCreateShader, glCompileShader, glCreateProgram, glLinkProgram
   */
  Shader(const char *vertexPath, const char *fragmentPath);

  /**
   * @brief Activates this shader program for rendering.
   * 
   * Makes this shader program the active program in the current OpenGL context.
   * All subsequent rendering operations will use this shader program until
   * another program is activated or the program is deactivated.
   * 
   * @pre OpenGL context must be current
   * @pre shaderProgram must contain a valid, linked OpenGL program
   * 
   * @post This shader program becomes the active program
   * @post All uniform variables and attributes are now accessible
   * @post Subsequent draw calls will use this shader
   * 
   * @note This is equivalent to calling glUseProgram(shaderProgram)
   * @note Should be called before setting uniforms or drawing
   * 
   * @see glUseProgram
   */
  void use() const { glUseProgram(shaderProgram); }

  /**
   * @brief Sets a 4-component float uniform variable in the shader.
   * 
   * Convenience function that combines uniform location lookup and value setting
   * in a single call. Queries the uniform location by name and sets the 4-component
   * float value if the uniform exists. Provides error checking and reporting.
   * 
   * @param name The exact name of the uniform variable as declared in shader source
   * @param v0   First component (x/red channel for colors, x for vectors)
   * @param v1   Second component (y/green channel for colors, y for vectors)
   * @param v2   Third component (z/blue channel for colors, z for vectors)
   * @param v3   Fourth component (w/alpha channel for colors, w for vectors)
   * 
   * @pre Shader program must be currently active (use() called)
   * @pre name must match a uniform variable declared in the shader source
   * @pre Uniform must be of type vec4, ivec4, or compatible 4-component type
   * 
   * @post If uniform exists, it is set to the specified 4-component value
   * @post If uniform doesn't exist, error message is logged to std::cerr
   * 
   * @note This function performs uniform location lookup on each call
   * @note For performance-critical code, consider caching uniform locations
   * @note Silent failure if uniform is optimized out by the compiler
   * 
   * @see glGetUniformLocation, glUniform4f
   * 
   * @example
   * @code
   * shader.use();
   * shader.setUniform4f("color", 1.0f, 0.0f, 0.0f, 1.0f); // Red color
   * shader.setUniform4f("position", x, y, z, w);           // 4D position
   * @endcode
   */
  void setUniform4f(const char *name, float v0, float v1, float v2,
                    float v3) const;

  /**
   * @brief Destructor that cleans up all OpenGL shader resources.
   * 
   * Automatically releases all OpenGL objects created by this shader:
   * - Deletes the vertex shader object
   * - Deletes the fragment shader object  
   * - Deletes the shader program object
   * 
   * This ensures proper resource cleanup following RAII principles.
   * 
   * @note OpenGL context must still be valid when destructor is called
   * @note Deletion is safe even if objects failed to create (0 IDs are ignored)
   * @note Called automatically when Shader object goes out of scope
   * 
   * @see glDeleteShader, glDeleteProgram
   */
  ~Shader();

  // Shader and program IDs (OpenGL handles) — publicly accessible for
  // inspection or debugging
  
  /** @brief OpenGL handle for the compiled vertex shader object */
  unsigned int vertexShader = 0;
  
  /** @brief OpenGL handle for the compiled fragment shader object */
  unsigned int fragmentShader = 0;
  
  /** @brief OpenGL handle for the linked shader program object */
  unsigned int shaderProgram = 0;

private:
  /** @brief Internal status flag for compilation/linking operations */
  int compileStatus{};
  
  /** @brief Buffer for storing compilation and linking error messages */
  char compilationLog[512]{};

  /**
   * @brief Internal function to check and report shader compilation status.
   * 
   * Queries the compilation status of a shader object and logs any compilation
   * errors to std::cerr. This function is used internally during the shader
   * compilation process to provide detailed error reporting.
   * 
   * @param shader OpenGL shader object ID to check (from glCreateShader)
   * 
   * @return 0 if compilation was successful, 1 if compilation failed
   * 
   * @note This function uses the internal compilationLog buffer
   * @note Error messages are automatically printed to std::cerr
   * @note Called internally by constructor during compilation process
   * 
   * @see glGetShaderiv, glGetShaderInfoLog
   */
  int checkCompileStatus(unsigned int shader);
};

// -----------------------------------------------------------------------------
// Inline Implementations
// -----------------------------------------------------------------------------

inline int Shader::checkCompileStatus(const unsigned int shader) {
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

  if (!compileStatus) {
    glGetShaderInfoLog(shader, 512, nullptr, compilationLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
              << compilationLog << std::endl;
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
    std::cerr << "ERROR::SHADER::PROGRAM_LINKING_FAILED\n"
              << compilationLog << std::endl;
  }

  if ((vertCompStatus + fragCompStatus + !linkStatus) > 0) {
    std::cerr << "ERROR::SHADER::FAILED_DURING_SETUP" << std::endl;
  }
}

inline void Shader::setUniform4f(const char *name, const float v0,
                                 const float v1, const float v2,
                                 const float v3) const {
  const int uniformLocation = glGetUniformLocation(shaderProgram, name);
  if (uniformLocation == -1) {
    std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
    return;
  }
  glUniform4f(uniformLocation, v0, v1, v2, v3);
}

inline Shader::~Shader() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
}
