/**
* @file lighting-maps.h
 * @brief
 *
 * @author Aaron Sharkey
 * @date 2025-08-24
 */

#pragma once

#include "example.h"
#include "mesh-generator.h"
#include "simple-mesh.h"
#include "shader.h"
#include "transform.h"
#include "cameras/fps-camera.h"


/**
 * @class LightingMapsExample
 * @brief
 */
class LightingMapsExample final : public Example {
public:
    LightingMapsExample() {
        cubeMesh.addPositionNormalTexAttributes();
        // Using same source vertices, but no use for normals
        lightSourceMesh.addPositionNormalTexAttributes();
        planeMesh.addPositionNormalAttributes();
    };

    void render(float deltaTime) override;

private:
    /**
     * @brief Cube vertex data with position coordinates and normals.
     * @note 1x1x1 cube centered at origin, 36 vertices (6 faces × 6 vertices)
     */
    float vertices[288] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    SimpleMesh cubeMesh{vertices, std::size(vertices), 36}; ///< Main lit object
    SimpleMesh lightSourceMesh{vertices, std::size(vertices), 36}; ///< Light source visualization
    SimpleMesh planeMesh = MeshGenerator::generatePlaneMesh(100, 1.0f, -0.75f, false, true);

    Texture diffuseMap{"resources/diffuse-maps/container.png"};
    Texture specularMap{"resources/specular-maps/container.png"};
    Texture emissiveMap{"resources/emissive-maps/matrix.jpg"};

    Shader lightingShader{
        "shaders/examples/lighting-maps/lighting-maps-shader.vert",
        "shaders/examples/lighting-maps/lighting-maps-shader.frag",
    };

    Shader lightSourceShader{
        "shaders/examples/lighting-maps/lighting-source-shader.vert",
        "shaders/examples/lighting-maps/lighting-source-shader.frag",
    };

    FPSCamera camera;
};

/**
 * @brief Render lit objects and light source with basic lighting model.
 * @param deltaTime Frame time for camera movement
 */
inline void LightingMapsExample::render(const float deltaTime) {

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
    // Use Specular + Diffuse Mapping
    lightingShader.setInt("mapped",1);
    lightingShader.bindTexture("material.diffuse", diffuseMap);
    lightingShader.bindTexture("material.specular", specularMap);
    lightingShader.bindTexture("material.emissive", emissiveMap);

    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
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

    // reset our texture so we don't diffuse map the plane
    lightingShader.resetTextureUnit();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 0);
    lightingShader.setInt("material.emissive", 0);

    // Don't use Specular + Diffuse Mapping
    lightingShader.setInt("mapped",0);
    // Render ground plane with lighting
    lightingShader.setVec3("materialPlane.diffuse", 0.0f, 0.5f, 0.5f);
    lightingShader.setVec3("materialPlane.specular", 0.5f, 0.5f, 0.5f);

    const glm::mat4 planeModel = Transform().getTransform();
    lightingShader.setMat4("model", planeModel);
    planeMesh.render();
}
