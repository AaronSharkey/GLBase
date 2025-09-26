/**
* @file light-casters.h
 * @brief
 *
 * @author Aaron Sharkey
 * @date 2025-08-27
 */

#pragma once
#include <iterator>

#include "example.h"
#include "simple-mesh.h"
#include "shader.h"
#include "texture.h"
#include "cameras/fps-camera.h"

class LightCastersExample : public Example {
public:
    LightCastersExample() {
        cubeMesh.addPositionNormalTexAttributes();
        lightSourceMesh.addPositionNormalTexAttributes();
    }

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

    // positions all containers
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    SimpleMesh cubeMesh{vertices, std::size(vertices), 36}; ///< Main lit object
    SimpleMesh lightSourceMesh{vertices, std::size(vertices), 36}; ///< Light source visualization

    Texture diffuseMap{"resources/diffuse-maps/container.png"};
    Texture specularMap{"resources/specular-maps/container.png"};

    Shader DirectionalLightingShader{
        "shaders/examples/light-casters/light-casters-shader.vert",
        "shaders/examples/light-casters/light-casters-shader.frag",
    };

    Shader PointLightingShader{
        "shaders/examples/light-casters/light-casters-shader.vert",
        "shaders/examples/light-casters/light-casters-point-light-shader.frag",
    };

    Shader SpotLightShader{
        "shaders/examples/light-casters/light-casters-shader.vert",
        "shaders/examples/light-casters/light-casters-spotlight-shader.frag",
    };

    Shader lightSourceShader{
        "shaders/examples/lighting-maps/lighting-source-shader.vert",
        "shaders/examples/lighting-maps/lighting-source-shader.frag",
    };

    Camera camera;

};

inline void LightCastersExample::render(float deltaTime) {

    camera.updateCameraPositions(deltaTime);
    // Set shared camera matrices
    const glm::mat4 view = camera.getViewMatrix();
    const glm::mat4 projection = camera.getProjectionMatrix();

    // // Render light source visualization
    // lightSourceShader.use();
    // lightSourceShader.setMat4("view", view);
    // lightSourceShader.setMat4("projection", projection);
    //
    // auto lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
    //
    // const Transform lightSourceModelTransform = Transform()
    //     .setPosition(lightPosition)
    //     .setScale(glm::vec3(0.2f));
    //
    // const glm::mat4 lightSourceModel = lightSourceModelTransform.getTransform();
    //
    // lightSourceShader.setMat4("model", lightSourceModel);
    // lightSourceMesh.render();

    // Render main cube with Directional lighting
    // DirectionalLightingShader.use();
    //
    // Directional light direction
    // DirectionalLightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    //
    // DirectionalLightingShader.bindTexture("material.diffuse", diffuseMap);
    // DirectionalLightingShader.bindTexture("material.specular", specularMap);
    // DirectionalLightingShader.setFloat("material.shininess", 32.0f);
    //
    // DirectionalLightingShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
    // DirectionalLightingShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    // DirectionalLightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    //
    // DirectionalLightingShader.setVec3("viewPos", camera.getCameraPosition());
    // DirectionalLightingShader.setMat4("view", view);
    // DirectionalLightingShader.setMat4("projection", projection);

    // PointLightingShader.use();
    //
    // PointLightingShader.bindTexture("material.diffuse", diffuseMap);
    // PointLightingShader.bindTexture("material.specular", specularMap);
    // PointLightingShader.setFloat("material.shininess", 32.0f);
    //
    // PointLightingShader.setVec3("light.position", lightPosition);
    //
    // PointLightingShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
    // PointLightingShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);
    // PointLightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    //
    // PointLightingShader.setFloat("light.constant",  1.0f);
    // PointLightingShader.setFloat("light.linear",    0.09f);
    // PointLightingShader.setFloat("light.quadratic", 0.032f);
    //
    // PointLightingShader.setVec3("viewPos", camera.getCameraPosition());
    // PointLightingShader.setMat4("view", view);
    // PointLightingShader.setMat4("projection", projection);

    SpotLightShader.use();

    SpotLightShader.bindTexture("material.diffuse", diffuseMap);
    SpotLightShader.bindTexture("material.specular", specularMap);
    SpotLightShader.setFloat("material.shininess", 32.0f);

    // Spotlight aka flashlight in this situation is coming from the player
    SpotLightShader.setVec3("light.position", camera.getCameraPosition());
    SpotLightShader.setVec3("light.direction", camera.getCameraFront());
    SpotLightShader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
    SpotLightShader.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));

    SpotLightShader.setVec3("light.ambient",  0.1f, 0.1f, 0.1f);
    SpotLightShader.setVec3("light.diffuse",  0.8f, 0.8f, 0.8f);
    SpotLightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    SpotLightShader.setFloat("light.constant",  1.0f);
    SpotLightShader.setFloat("light.linear",    0.09f);
    SpotLightShader.setFloat("light.quadratic", 0.032f);

    SpotLightShader.setVec3("viewPos", camera.getCameraPosition());
    SpotLightShader.setMat4("view", view);
    SpotLightShader.setMat4("projection", projection);

    for (const auto& position: cubePositions) {
        const glm::mat4 objectModel = Transform()
        .setPosition(position)
        .getTransform();

        SpotLightShader.setMat4("model", objectModel);
        cubeMesh.render();
    }

}


