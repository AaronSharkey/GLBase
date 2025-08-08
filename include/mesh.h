/**
 * @file mesh.h
 * @brief OpenGL mesh management with VAO/VBO/EBO and wireframe support.
 *
 * @author Aaron Sharkey
 * @date 2025-07-15
 */

#pragma once

#include <glad/glad.h>
#include <stdexcept>

/**
 * @class Mesh
 * @brief VAO-backed mesh with indexed rendering and wireframe mode.
 */
class Mesh {
public:
    /**
     * @brief Create indexed mesh with VBO and EBO.
     * @param vertices Vertex data array
     * @param indices Index data array
     * @param elementCount Total floats in vertex array
     * @param indexCount Number of indices
     * @param wireframe Enable wireframe rendering
     */
    explicit Mesh(const GLfloat *vertices, const GLuint *indices, size_t elementCount,
                  size_t indexCount, bool wireframe = false);

    /**
     * @brief Create non-indexed mesh with VBO only.
     * @param vertices Vertex data array
     * @param elementCount Total floats in vertex array
     * @param vertexCount Number of vertices
     * @param wireframe Enable wireframe rendering
     */
    explicit Mesh(const GLfloat *vertices, size_t elementCount, int vertexCount, bool wireframe = false);

    /**
     * @brief Render mesh using glDrawElements or glDrawArrays.
     * @throws std::runtime_error if attributes not set
     */
    void render() const;

    /**
     * @brief Set position attributes (3 floats per vertex).
     */
    void addPositionOnlyAttributes();

    /**
     * @brief Set position + color attributes (6 floats per vertex).
     */
    void addPositionColorAttributes();

    /**
     * @brief Set position + texture attributes (5 floats per vertex).
     */
    void addPositionTexAttributes();

    /**
     * @brief Set position + color + texture attributes (8 floats per vertex).
     */
    void addPositionColorTexAttributes();

    /**
     * @brief Clean up OpenGL resources.
     */
    ~Mesh();

private:
    const GLfloat *vertices; ///< Vertex data pointer (not owned)
    const GLuint *indices; ///< Index data pointer (not owned)
    unsigned int VAO = 0; ///< Vertex Array Object ID
    unsigned int VBO = 0; ///< Vertex Buffer Object ID
    unsigned int EBO = 0; ///< Element Buffer Object ID
    size_t elementCount = 0; ///< Total floats in vertex array
    size_t indexCount = 0; ///< Number of indices
    int vertexCount = 0; ///< Number of vertices
    bool wireframe = false; ///< Wireframe rendering mode
    bool vertexAttributesSet = false; ///< Prevent render before attributes set

    /**
     * @brief Configure vertex attribute pointer.
     * @param index Attribute location
     * @param size Components per vertex (1-4)
     * @param stride Bytes between vertices
     * @param offset Byte offset to attribute
     * @param normalized Normalize fixed-point data
     */
    void addVertexAttributes(GLint index, GLint size, GLint stride, GLint offset, GLboolean normalized = GL_FALSE);
};

// -----------------------------------------------------------------------------
// Mesh Implementation
// -----------------------------------------------------------------------------

inline Mesh::Mesh(const GLfloat *const vertices, const size_t elementCount, int vertexCount, const bool wireframe)
    : vertices(vertices), indices(nullptr), elementCount(elementCount), vertexCount(vertexCount), wireframe(wireframe) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(elementCount * sizeof(GLfloat)),
                 vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

inline Mesh::Mesh(const GLfloat *const vertices, const GLuint *indices, const size_t elementCount,
                  const size_t indexCount, const bool wireframe)
    : vertices(vertices), indices(indices),
      elementCount(elementCount), indexCount(indexCount),
      wireframe(wireframe) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(elementCount * sizeof(GLfloat)),
                 vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indexCount * sizeof(GLuint)),
                 indices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

inline void Mesh::render() const {
    if (!vertexAttributesSet) {
        throw std::runtime_error("ERROR::MESH::Attempted to render with uninitialized Vertex Attributes.");
    }

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glBindVertexArray(VAO);

    if (EBO) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
    } else {
        if (vertexCount == 0) {
            throw std::runtime_error(
                "ERROR::MESH::Attempted to render with `glDrawArrays` but vertexCount was not set.");
        }
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    glBindVertexArray(0);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

inline void Mesh::addVertexAttributes(const GLint index, const GLint size, const GLint stride, const GLint offset,
                                      const GLboolean normalized) {
    if (size < 1 || size > 4) {
        throw std::invalid_argument("ERROR::MESH::Invalid vertex attribute size. Must be 1, 2, 3, or 4.");
    }

    if (index < 0) {
        throw std::invalid_argument("ERROR::MESH::Invalid vertex attribute index. Must be >= 0.");
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(index, size, GL_FLOAT, normalized, stride * sizeof(GLfloat),
                          reinterpret_cast<void *>(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    vertexAttributesSet = true;
}

inline void Mesh::addPositionOnlyAttributes() {
    addVertexAttributes(0, 3, 3, 0); // Position: 3 floats, stride 3, offset 0
}

inline void Mesh::addPositionColorAttributes() {
    addVertexAttributes(0, 3, 6, 0); // Position: 3 floats, stride 6, offset 0
    addVertexAttributes(1, 3, 6, 3); // Color: 3 floats, stride 6, offset 3
}

inline void Mesh::addPositionTexAttributes() {
    addVertexAttributes(0, 3, 5, 0); // Position: 3 floats, stride 5, offset 0
    addVertexAttributes(1, 2, 5, 3); // Tex coords: 2 floats, stride 5, offset 3
}

inline void Mesh::addPositionColorTexAttributes() {
    addVertexAttributes(0, 3, 8, 0); // Position: offset 0
    addVertexAttributes(1, 3, 8, 3); // Color: offset 3
    addVertexAttributes(2, 2, 8, 6); // Tex coords: offset 6
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
