/**
 * @file mesh.h
 * @brief Declaration and implementation of the Mesh class for OpenGL mesh
 * management. Handles creation, binding, and rendering of a VAO-backed mesh
 * using VBO and EBO. Supports optional wireframe rendering for debugging or
 * stylistic display.
 *
 * @author Aaron Sharkey
 * @date 2025-07-15
 */

#pragma once

#include <glad/glad.h>
#include <stdexcept>

/**
 * @class Mesh
 * @brief Comprehensive OpenGL mesh management with modern buffer objects.
 * 
 * The Mesh class provides a complete solution for managing indexed geometry
 * in OpenGL using Vertex Array Objects (VAO), Vertex Buffer Objects (VBO),
 * and Element Buffer Objects (EBO). It supports flexible vertex layouts and
 * optional wireframe rendering for debugging and stylistic effects.
 * 
 * Key Features:
 * - VAO-based geometry management for efficient state binding
 * - Indexed rendering using EBO for memory efficiency
 * - Configurable vertex stride for flexible attribute layouts
 * - Optional wireframe rendering mode
 * - RAII-based OpenGL resource management
 * - Exception-safe rendering with validation
 * 
 * Vertex Data Requirements:
 * - Vertex data must remain valid for the lifetime of the Mesh object
 * - Index data must remain valid for the lifetime of the Mesh object
 * - Currently assumes position-only vertices (3 floats per vertex)
 * - Supports any index count for flexible geometry complexity
 * 
 * @note Uses modern OpenGL practices with VAO state management
 * @note Caller retains ownership of vertex and index data arrays
 * @note Automatic cleanup of all OpenGL resources on destruction
 * 
 * @see OpenGL VAO specification
 * @see glGenVertexArrays, glGenBuffers, glDrawElements
 */
class Mesh {
public:
  /**
   * @brief Constructs a mesh and uploads geometry data to GPU buffers.
   * 
   * Creates a complete mesh with VAO, VBO, and EBO setup. Uploads vertex and
   * index data to GPU memory and configures vertex attribute layout. The mesh
   * is immediately ready for rendering after construction.
   * 
   * @param vertices    Pointer to vertex data array (caller retains ownership)
   * @param indices     Pointer to index data array (caller retains ownership)
   * @param vertexCount Total number of float components in vertex array
   * @param indexCount  Number of indices for indexed drawing
   * @param stride      Number of floats per vertex (attribute stride)
   * @param wireframe   Enable wireframe rendering mode (default: false)
   * 
   * @pre vertices != nullptr
   * @pre indices != nullptr  
   * @pre vertexCount > 0
   * @pre indexCount > 0
   * @pre stride > 0
   * @pre OpenGL context must be current
   * 
   * @post VAO, VBO, and EBO are created and configured
   * @post Vertex data is uploaded to GPU memory
   * @post Index data is uploaded to GPU memory
   * @post Vertex attribute 0 is configured for position data
   * @post Mesh is ready for rendering
   * 
   * @note Currently hardcoded for position-only vertices (3 components)
   * @note Vertex and index data must remain valid during mesh lifetime
   * @note Uses GL_STATIC_DRAW for buffer usage hint
   * 
   * @throws std::runtime_error if OpenGL buffer creation fails
   * 
   * @example
   * @code
   * float vertices[] = {0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};
   * GLuint indices[] = {0, 1, 2};
   * Mesh triangle(vertices, indices, 9, 3, 3);  // Triangle mesh
   * @endcode
   */
  explicit Mesh(const GLfloat *vertices, const GLuint *indices,
                size_t vertexCount, size_t indexCount, int stride,
                bool wireframe = false);

  /**
   * @brief Renders the mesh using indexed drawing.
   * 
   * Performs the complete rendering operation for this mesh:
   * 1. Optionally enables wireframe mode
   * 2. Binds the VAO (automatically binds VBO and EBO)
   * 3. Issues indexed draw call
   * 4. Unbinds VAO for clean state
   * 5. Restores fill mode if wireframe was enabled
   * 
   * @pre OpenGL context must be current
   * @pre A shader program must be active (glUseProgram called)
   * @pre EBO must be properly initialized (checked internally)
   * 
   * @post Mesh geometry is rendered to current framebuffer
   * @post OpenGL state is restored (VAO unbound, polygon mode reset)
   * 
   * @throws std::runtime_error if EBO is uninitialized (0)
   * 
   * @note Uses GL_TRIANGLES primitive type
   * @note Wireframe mode temporarily changes polygon mode globally
   * @note Performance: VAO binding caches all vertex state efficiently
   * 
   * @see glBindVertexArray, glDrawElements, glPolygonMode
   */
  void render() const;

  /**
   * @brief Destructor that releases all OpenGL resources.
   * 
   * Automatically cleans up all OpenGL objects created by this mesh:
   * - Deletes Element Buffer Object (EBO)
   * - Deletes Vertex Buffer Object (VBO)
   * - Deletes Vertex Array Object (VAO)
   * 
   * Follows RAII principles for automatic resource management.
   * 
   * @note OpenGL context must still be valid when destructor is called
   * @note Safe to call even if buffer creation failed (0 IDs ignored)
   * @note Called automatically when Mesh object goes out of scope
   * @note IDs are reset to 0 after deletion for safety
   * 
   * @see glDeleteBuffers, glDeleteVertexArrays
   */
  ~Mesh();

private:
  /** @brief Pointer to external vertex data array (not owned by this class) */
  const GLfloat *vertices;
  
  /** @brief Pointer to external index data array (not owned by this class) */
  const GLuint *indices;
  
  /** @brief OpenGL Vertex Array Object handle (manages vertex state) */
  unsigned int VAO = 0;
  
  /** @brief OpenGL Vertex Buffer Object handle (stores vertex data) */
  unsigned int VBO = 0;
  
  /** @brief OpenGL Element Buffer Object handle (stores index data) */
  unsigned int EBO = 0;
  
  /** @brief Total number of float components in the vertex array */
  size_t vertexCount = 0;
  
  /** @brief Number of indices used for indexed rendering */
  size_t indexCount = 0;
  
  /** @brief Flag indicating whether to render in wireframe mode */
  bool wireframe = false;
};

// -----------------------------------------------------------------------------
// Mesh Implementation
// -----------------------------------------------------------------------------

inline Mesh::Mesh(const GLfloat *const vertices, const GLuint *indices,
                  const size_t vertexCount, const size_t indexCount,
                  const int stride, const bool wireframe)
    : vertices(vertices), indices(indices), vertexCount(vertexCount),
      indexCount(indexCount), wireframe(wireframe) {
  // Generate OpenGL buffer objects
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // VAO scope
  glBindVertexArray(VAO);
  {
    // VBO scope
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    {
      glBufferData(GL_ARRAY_BUFFER,
                   static_cast<GLsizeiptr>(vertexCount * sizeof(GLfloat)),
                   vertices, GL_STATIC_DRAW);

      // Bind and fill EBO
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   static_cast<GLsizeiptr>(indexCount * sizeof(GLuint)),
                   indices, GL_STATIC_DRAW);

      // Define layout: assume position-only for now
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            static_cast<GLsizei>(stride * sizeof(float)),
                            static_cast<void *>(nullptr));
      glEnableVertexAttribArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO unbound (EBO stays bound to VAO)
  }
  glBindVertexArray(0); // Unbind VAO
}

inline void Mesh::render() const {
  if (EBO == 0) {
    throw std::runtime_error(
        "ERROR::MESH::Attempted to render with uninitialized EBO.");
  }

  if (wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount),
                 GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);

  if (wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

inline Mesh::~Mesh() {
  if (EBO != 0) {
    glDeleteBuffers(1, &EBO);
    EBO = 0;
  }

  if (VBO != 0) {
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }

  if (VAO != 0) {
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }
}
