/**
 * @file spinning-block.h
 * @brief Dual spinning textured squares example.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once

#include "example.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class SpinningBlock
 * @brief Two textured squares with different rotation patterns.
 */
class SpinningBlock final : public Example {
public:
    SpinningBlock() {
        squareMesh.addPositionTexAttributes();
    }

    void render(float deltaTime) override;

private:
    /**
     * @brief Square vertex data with position and texture coordinates.
     * @note 1x1 square centered at origin, made from 2 triangles
     */
    float vertices[20] = {
        // positions          // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left
   };

    const GLuint indices[6] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    Mesh squareMesh{vertices, indices, std::size(vertices), std::size(indices)};

    Texture containerTexture{"resources/textures/container.jpg"};
    Texture awesomeFaceTexture{"resources/textures/awesome-face.png", {.flipVertically = true}};

    Shader squareShader{
        "shaders/examples/spinning-block/spinning-block.vert",
        "shaders/examples/spinning-block/spinning-block.frag",
    };
};

/**
 * @brief Render two textured squares with different rotation patterns.
 * @param deltaTime Frame time (unused, uses glfwGetTime for animation)
 */
inline void SpinningBlock::render(float deltaTime) {
    squareShader.use();
    squareShader.bindTexture("containerTexture", containerTexture);
    squareShader.bindTexture("awesomefaceTexture", awesomeFaceTexture);

    const auto currentTime = static_cast<float>(glfwGetTime());

    // First square: upper position with XZ rotation
    const glm::mat4 transform1 = Transform()
        .setPosition(glm::vec3(0.0f, 0.5f, 0.0f))
        .setRotation(glm::vec3(currentTime, 0.0f, currentTime))
        .setScale(glm::vec3(0.5f, 0.5f, 1.0f))
        .getTransform();

    squareShader.setMat4("transform", transform1);
    squareMesh.render();

    // Second square: lower position with Y and negative Z rotation
    const glm::mat4 transform2 = Transform()
        .setPosition(glm::vec3(0.0f, -0.5f, 0.0f))
        .setRotation(glm::vec3(0.0f, currentTime, -currentTime))
        .setScale(glm::vec3(0.5f, 0.5f, 1.0f))
        .getTransform();

    squareShader.setMat4("transform", transform2);
    squareMesh.render();
}
