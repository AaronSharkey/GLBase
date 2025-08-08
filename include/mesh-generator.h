/**
 * @file mesh-generator.h
 * @brief Procedural mesh generation utilities.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once

#include "mesh.h"

/**
 * @class MeshGenerator
 * @brief Static utility for generating common mesh types.
 */
class MeshGenerator {
public:
    /**
     * @brief Generate a grid plane mesh centered at origin.
     * @param gridSize Number of grid divisions per axis
     * @param gridSpacing Distance between grid vertices
     * @param height Y-coordinate for all vertices
     * @param wireframe Enable wireframe rendering
     * @return Indexed mesh with position-only vertices
     */
    static Mesh generatePlaneMesh(int gridSize = 20, float gridSpacing = 1.0f, float height = 0.0f,
                                  bool wireframe = false);
};

inline Mesh MeshGenerator::generatePlaneMesh(const int gridSize, const float gridSpacing, const float height,
                                             const bool wireframe) {
    const int vertexCount = (gridSize + 1) * (gridSize + 1);
    int vertexIndex = 0;
    int indexIndex = 0;

    // Vertex data: x, y, z per vertex
    GLfloat vertices[vertexCount * 3];

    // Index data: 2 triangles per grid square
    GLuint indices[gridSize * gridSize * 2 * 3];

    const int planeStart = ((gridSize / 2) * -1);
    const int planeEnd = (gridSize / 2);

    // Generate vertices in grid pattern
    for (int i = planeStart; i <= planeEnd; i++) {
        for (int j = planeStart; j <= planeEnd; j++) {
            const float x = gridSpacing * i;
            const float y = height;
            const float z = gridSpacing * j;

            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = z;
        }
    }

    // Generate triangle indices for each grid square
    for (int squareRow = 0; squareRow < gridSize; squareRow++) {
        for (int squareCol = 0; squareCol < gridSize; squareCol++) {
            // Calculate vertex indices for current square
            const int topLeft = squareRow * (gridSize + 1) + squareCol;
            const int topRight = squareRow * (gridSize + 1) + (squareCol + 1);
            const int bottomLeft = (squareRow + 1) * (gridSize + 1) + squareCol;
            const int bottomRight = (squareRow + 1) * (gridSize + 1) + (squareCol + 1);

            // Triangle 1: topLeft -> topRight -> bottomLeft (CCW)
            indices[indexIndex++] = topLeft;
            indices[indexIndex++] = topRight;
            indices[indexIndex++] = bottomLeft;

            // Triangle 2: topRight -> bottomRight -> bottomLeft (CCW)
            indices[indexIndex++] = topRight;
            indices[indexIndex++] = bottomRight;
            indices[indexIndex++] = bottomLeft;
        }
    }

    const size_t vertexArraySize = vertexCount * 3;
    const size_t indexArraySize = gridSize * gridSize * 2 * 3;

    return Mesh(vertices, indices, vertexArraySize, indexArraySize, wireframe);
}
