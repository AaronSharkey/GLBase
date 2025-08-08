/**
* @file textured-block.h
 * @brief Static textured square example.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once
#include "example.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "glad/glad.h"

/**
 * @class TexturedBlock
 * @brief Single textured square with no animation.
 */
class TexturedBlock final : public Example {
public:
    TexturedBlock() {
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
        "shaders/examples/textured-block/textured-block.vert",
        "shaders/examples/textured-block/textured-block.frag",
    };
};

/**
 * @brief Render static textured square.
 * @param deltaTime Frame time (unused for static rendering)
 */
inline void TexturedBlock::render(float deltaTime) {
    squareShader.use();
    squareShader.bindTexture("containerTexture", containerTexture);
    squareShader.bindTexture("awesomefaceTexture", awesomeFaceTexture);
    squareMesh.render();
}
