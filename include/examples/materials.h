/**
* @file materials.h
 * @brief
 *
 * @author Aaron Sharkey
 * @date 2025-08-21
 */

#pragma once

#include "example.h"
#include "mesh-generator.h"
#include "simple-mesh.h"
#include "shader.h"
#include "transform.h"
#include "cameras/fps-camera.h"


/**
 * @class MaterialExample
 * @brief
 */
class MaterialExample final : public Example {
public:
    MaterialExample() {
        cubeMesh.addPositionNormalAttributes();
        // Using same source vertices, but no use for normals
        lightSourceMesh.addPositionNormalAttributes();
        planeMesh.addPositionNormalAttributes();
    };

    void render(float deltaTime) override;

private:
    /**
     * @brief Cube vertex data with position coordinates and normals.
     * @note 1x1x1 cube centered at origin, 36 vertices (6 faces × 6 vertices)
     */
    float vertices[216] = {
        // Back Face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        // Front Face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        // Left Face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        // Right Face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        // Bottom Face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        // Top Face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    SimpleMesh cubeMesh{vertices, std::size(vertices), 36}; ///< Main lit object
    SimpleMesh lightSourceMesh{vertices, std::size(vertices), 36}; ///< Light source visualization
    SimpleMesh planeMesh = MeshGenerator::generatePlaneMesh(100, 1.0f, -0.75f, false, true);

    Shader lightingShader{
        "shaders/examples/materials/materials-lighting-shader.vert",
        "shaders/examples/materials/materials-lighting-shader.frag",
    };

    Shader lightSourceShader{
        "shaders/examples/materials/lighting-source-shader.vert",
        "shaders/examples/materials/lighting-source-shader.frag",
    };

    FPSCamera camera;
};

/**
 * @brief Render lit objects and light source with basic lighting model.
 * @param deltaTime Frame time for camera movement
 */
inline void MaterialExample::render(float deltaTime) {
    camera.updateCameraPositions(deltaTime);

    // Set shared camera matrices
    const glm::mat4 view = camera.getViewMatrix();
    const glm::mat4 projection = camera.getProjectionMatrix();

    // Render light source visualization
    lightSourceShader.use();
    lightSourceShader.setMat4("view", view);
    lightSourceShader.setMat4("projection", projection);

    // Have it rotate around the origin
    static float angle = 0.0f;
    float radius = 2.5f;
    float rotationStep = 1.0f;

    angle += rotationStep * deltaTime;
    const auto orbitCenter = glm::vec3(0.0f, 0.5f, 0.0f);

    glm::vec3 lightPosition = orbitCenter + glm::vec3(
                                  radius * cos(angle),
                                  0.0f,
                                  radius * sin(angle)
                                  );

    const Transform lightSourceModelTransform = Transform()
                                                .setPosition(lightPosition)
                                                .setScale(glm::vec3(0.2f));

    const glm::mat4 lightSourceModel = lightSourceModelTransform.getTransform();

    lightSourceShader.setMat4("model", lightSourceModel);
    lightSourceMesh.render();

    // Render main cube with lighting
    lightingShader.use();

    lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("light.position", lightSourceModelTransform.getPosition());

    lightingShader.setVec3("viewPos", camera.getCameraPosition());
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);

    const glm::mat4 objectModel = Transform()
                                  .setPosition(glm::vec3(0.0f, 0.5f, 0.0f))
                                  .getTransform();

    lightingShader.setMat4("model", objectModel);
    cubeMesh.render();

    // Render ground plane with lighting
    // lightingShader.setVec3("objectColor", 0.0f, 0.5f, 0.5f); // Teal plane
    lightingShader.setVec3("material.ambient", 0.0f, 0.5f, 0.5f);
    lightingShader.setVec3("material.diffuse", 0.0f, 0.5f, 0.5f);
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    const glm::mat4 planeModel = Transform().getTransform();
    lightingShader.setMat4("model", planeModel);
    planeMesh.render();
}
