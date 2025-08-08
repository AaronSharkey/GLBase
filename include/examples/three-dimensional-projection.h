/**
 * @file three-dimensional-projection.h
 * @brief 3D perspective projection example with dual spinning cubes.
 *
 * @author Aaron Sharkey
 * @date 2025-07-20
 */

#pragma once

#include "example.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.h"
#include "window.h"

/**
 * @class ThreeDimensionalProjection
 * @brief Two textured cubes with different rotations in 3D space.
 */
class ThreeDimensionalProjection final : public Example {
public:
    ThreeDimensionalProjection() {
        cubeMesh.addPositionTexAttributes();
    }

    void render(float deltaTime) override;

private:
    /**
     * @brief Cube vertex data with position and texture coordinates.
     * @note 1x1x1 cube centered at origin, 36 vertices (6 faces × 6 vertices)
     */
    float vertices[180] = {
        // BACK FACE (z = -0.5) - Looking towards negative Z
        // Triangle 1: bottom-left → bottom-right → top-right
        // Triangle 2: top-right → top-left → bottom-left
        //     x      y      z     u     v
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right (repeated)
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left (repeated)

        // FRONT FACE (z = +0.5) - Looking towards positive Z
        // Triangle 1: bottom-left → bottom-right → top-right
        // Triangle 2: top-right → top-left → bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right (repeated)
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left (repeated)

        // LEFT FACE (x = -0.5) - Looking towards negative X
        // Triangle 1: front-top → back-top → back-bottom
        // Triangle 2: back-bottom → front-bottom → front-top
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // front-top
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // back-top
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // back-bottom
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // back-bottom (repeated)
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // front-bottom
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // front-top (repeated)

        // RIGHT FACE (x = +0.5) - Looking towards positive X
        // Triangle 1: front-top → back-top → back-bottom
        // Triangle 2: back-bottom → front-bottom → front-top
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // front-top
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // back-top
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // back-bottom
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // back-bottom (repeated)
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // front-bottom
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // front-top (repeated)

        // BOTTOM FACE (y = -0.5) - Looking towards negative Y
        // Triangle 1: back-left → back-right → front-right
        // Triangle 2: front-right → front-left → back-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // back-left
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // back-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // front-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // front-right (repeated)
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // front-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // back-left (repeated)

        // TOP FACE (y = +0.5) - Looking towards positive Y
        // Triangle 1: back-left → back-right → front-right
        // Triangle 2: front-right → front-left → back-left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // back-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // back-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // front-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // front-right (repeated)
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, // front-left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f // back-left (repeated)
    };

    Mesh cubeMesh{vertices, std::size(vertices), 36};

    Texture containerTexture{"resources/textures/container.jpg"};
    Texture awesomeFaceTexture{"resources/textures/awesome-face.png", {.flipVertically = true}};
    Shader cubeShader{
        "shaders/examples/three-dimensional-projection/three-dimensional-projection.vert",
        "shaders/examples/three-dimensional-projection/three-dimensional-projection.frag",
    };
};

/**
 * @brief Render two cubes with different positions and rotations in 3D space.
 * @param deltaTime Frame time (unused, uses glfwGetTime for animation)
 */
inline void ThreeDimensionalProjection::render(float deltaTime) {
    cubeShader.use();
    cubeShader.bindTexture("containerTexture", containerTexture);
    cubeShader.bindTexture("awesomefaceTexture", awesomeFaceTexture);

    const auto currentTime = static_cast<float>(glfwGetTime());

    // Static camera setup (shared for both cubes)
    const glm::mat4 view = Transform()
            .setPosition(glm::vec3(0.0f, 0.0f, -3.0f))
            .getTransform();

    const glm::mat4 projection = glm::perspective(glm::radians(90.0f),
                                                  static_cast<float>(SVGA_WIDTH) / static_cast<float>(SVGA_HEIGHT),
                                                  0.1f, 100.0f);

    cubeShader.setMat4("view", view);
    cubeShader.setMat4("projection", projection);

    // First cube: upper-left with XY rotation
    const glm::mat4 model1 = Transform()
            .setPosition(glm::vec3(-1.0f, 1.0f, 0.0f))
            .setRotation(glm::vec3(currentTime * 0.5f, currentTime * -1.5f, 0.0f))
            .getTransform();

    cubeShader.setMat4("model", model1);
    cubeMesh.render();

    // Second cube: lower-right with Z rotation only
    const glm::mat4 model2 = Transform()
        .setPosition(glm::vec3(1.0f, -1.0f, 0.0f))
        .setRotation(glm::vec3(0.0f, 0.0f, currentTime * 0.5f))
        .getTransform();

    cubeShader.setMat4("model", model2);
    cubeMesh.render();
}
