/**
 * @file mesh.h
 * @brief Declaration and implementation of the Mesh class for OpenGL mesh management.
 *        Handles creation, binding, and rendering of a VAO-backed mesh using VBO and EBO.
 *        Optionally supports wireframe rendering for debugging or stylistic purposes.
 *
 * @author Aaron Sharkey
 * @date 2025-07-15
 */

#pragma once

#include <glad/glad.h>
#include <iostream>

/**
 * @class Mesh
 * @brief Manages an indexed OpenGL mesh with support for VAO/VBO/EBO and optional wireframe rendering.
 */
class Mesh {
public:
    /**
     * @brief Constructs the mesh and uploads vertex/index data to the GPU.
     *
     * @param vertices      Pointer to raw vertex data (caller-managed).
     * @param indices       Pointer to raw index data (caller-managed).
     * @param vertexCount   Number of float components in vertex array (not number of vertices).
     * @param indexCount    Number of indices for glDrawElements.
     * @param stride        Number of floats per vertex.
     * @param wireframe     If true, this mesh will render in wireframe mode.
     */
    explicit Mesh(const GLfloat* vertices, const GLint* indices, size_t vertexCount, size_t indexCount, int stride, bool wireframe = false);

    /**
     * @brief Renders the mesh using glDrawElements.
     */
    void render() const;

    /**
     * @brief Releases GPU resources on destruction.
     */
    ~Mesh();

private:
    const GLfloat* vertices;   ///< Pointer to raw vertex data (lifetime managed by caller).
    const GLint* indices;      ///< Pointer to raw index data (lifetime managed by caller).
    unsigned int VAO;          ///< Vertex Array Object ID.
    unsigned int VBO;          ///< Vertex Buffer Object ID.
    unsigned int EBO;          ///< Element Buffer Object ID.
    size_t vertexCount;        ///< Total number of float components in vertex array.
    size_t indexCount;         ///< Number of indices for rendering.
    bool wireframe;            ///< Whether to render the mesh in wireframe mode.
};

// -----------------------------------------------------------------------------
// Mesh Implementation
// -----------------------------------------------------------------------------

inline Mesh::Mesh(const GLfloat* const vertices, const GLint* indices, const size_t vertexCount, const size_t indexCount, const int stride, bool wireframe)
    : VAO(0), VBO(0), EBO(0)
{
    this->vertices = vertices;
    this->indices = indices;
    this->vertexCount = vertexCount;
    this->indexCount = indexCount;
    this->wireframe = wireframe;

    // Generate OpenGL objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Begin VAO scope
    glBindVertexArray(VAO);
    {
        // Begin VBO scope
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        {
            // Upload vertex data to GPU
            glBufferData(GL_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(vertexCount * sizeof(this->vertices[0])),
                         this->vertices,
                         GL_STATIC_DRAW);

            // Bind and upload index data to EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(indexCount * sizeof(this->indices[0])),
                         this->indices,
                         GL_STATIC_DRAW);

            // Define vertex attribute layout for position (location = 0)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                  static_cast<GLsizei>(sizeof(float) * stride),
                                  static_cast<void*>(nullptr));
            glEnableVertexAttribArray(0);
        }
        // End VBO scope
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    // End VAO scope
    glBindVertexArray(0);
}

inline void Mesh::render() const {
    if (EBO == 0) {
        std::cerr << "Error: Attempted to render mesh with uninitialized EBO." << std::endl;
        exit(-1);
    }

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
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
