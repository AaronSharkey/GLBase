/**
 * @file camera-with-movement.h
 * @brief FPS camera example with textured cube and wireframe ground plane.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once
#include "example.h"
#include "simple-mesh.h"
#include "shader.h"
#include "texture.h"
#include "mesh-generator.h"
#include "cameras/fps-camera.h"

/**
 * @class CameraWithMovement
 * @brief Spinning textured cube with ground plane and FPS camera controls.
 */
class CameraWithMovement final : public Example {
public:
    CameraWithMovement() {
        cubeMesh.addPositionTexAttributes();
        planeMesh.addPositionOnlyAttributes();
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

    SimpleMesh cubeMesh{vertices, std::size(vertices), 36};
    SimpleMesh planeMesh = MeshGenerator::generatePlaneMesh(100, 1.0f, -0.75f, true);

    Texture containerTexture{"resources/textures/container.jpg"};
    Texture awesomeFaceTexture{"resources/textures/awesome-face.png", {.flipVertically = true}};

    Shader cubeShader{
        "shaders/examples/camera-with-movement/camera-with-movement.vert",
        "shaders/examples/camera-with-movement/camera-with-movement.frag",
    };

    FPSCamera camera;
};

/**
 * @brief Render spinning cube and static ground plane with FPS camera.
 * @param deltaTime Frame time for camera movement
 */
inline void CameraWithMovement::render(const float deltaTime) {
    // Update camera from input (disabled in debug mode)
    camera.updateCameraPositions(deltaTime);

    cubeShader.use();
    cubeShader.bindTexture("containerTexture", containerTexture);
    cubeShader.bindTexture("awesomefaceTexture", awesomeFaceTexture);

    // Set camera matrices (same for both objects)
    cubeShader.setMat4("view", camera.getViewMatrix());
    cubeShader.setMat4("projection", camera.getProjectionMatrix());

    const auto currentTime = static_cast<float>(glfwGetTime());

    // Render spinning cube
    const glm::mat4 cubeModel = Transform()
            .setPosition(glm::vec3(0.0f, 0.5f, 0.0f))
            .setRotation(glm::vec3(currentTime, 0.0f, currentTime))
            .getTransform();

    cubeShader.setMat4("model", cubeModel);
    cubeMesh.render();

    // Render static ground plane
    const glm::mat4 planeModel = Transform().getTransform(); // Identity matrix
    cubeShader.setMat4("model", planeModel);
    planeMesh.render();
}
