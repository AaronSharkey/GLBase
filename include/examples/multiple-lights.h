#pragma once
#include <vector>
#include <format>

#include "example.h"
#include "simple-mesh.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

class MultipleLightsExample : public Example {
public:
    MultipleLightsExample() {
        cubeMesh.addPositionNormalTexAttributes();
        lightSourceMesh.addPositionNormalTexAttributes();
    }

    void render(float deltaTime) override;

private:
    float vertices[288] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    // positions all containers
    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    std::vector<glm::vec3> pointLightPositions = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

    SimpleMesh cubeMesh{vertices, std::size(vertices), 36}; ///< Main lit object
    SimpleMesh lightSourceMesh{vertices, std::size(vertices), 36}; ///< Light source visualization

    Texture diffuseMap{"resources/diffuse-maps/container.png"};
    Texture specularMap{"resources/specular-maps/container.png"};

    Shader comboLightShader{
        "shaders/examples/multiple-lights/multiple-lights-shader.vert",
        "shaders/examples/multiple-lights/multiple-lights-shader.frag",
    };

    Shader lightSourceShader{
        "shaders/examples/lighting-maps/lighting-source-shader.vert",
        "shaders/examples/lighting-maps/lighting-source-shader.frag",
    };

    Camera camera;
};

inline void MultipleLightsExample::render(float deltaTime) {
    camera.updateCameraPositions(deltaTime);
    // Set shared camera matrices
    const glm::mat4 view = camera.getViewMatrix();
    const glm::mat4 projection = camera.getProjectionMatrix();

    // Render Light Sources
    lightSourceShader.use();
    lightSourceShader.setMat4("view", view);
    lightSourceShader.setMat4("projection", projection);

    for (const auto& position : pointLightPositions) {
        const glm::mat4 objectModel = Transform()
                                      .setPosition(position)
                                      .setScale(glm::vec3(0.2f))
                                      .getTransform();

        lightSourceShader.setMat4("model", objectModel);
        lightSourceMesh.render();
    }

    comboLightShader.use();

    // Generic uniforms
    comboLightShader.setVec3("viewPos", camera.getCameraPosition());
    comboLightShader.setMat4("view", view);
    comboLightShader.setMat4("projection", projection);

    // Material
    comboLightShader.bindTexture("material.diffuse", diffuseMap);
    comboLightShader.bindTexture("material.specular", specularMap);
    comboLightShader.setFloat("material.shininess", 32.0f);

    // Directional Light
    comboLightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);

    // Light properties
    comboLightShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    comboLightShader.setVec3("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
    comboLightShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    // Point Light
    int index = 0;
    for (const auto& position : pointLightPositions) {
        std::string uniformPrefix = std::format("pointLights[{}]", index);

        // Position
        comboLightShader.setVec3((uniformPrefix + ".position").c_str(), position);

        // Point light calculation
        comboLightShader.setFloat((uniformPrefix + ".constant").c_str(), 1.0f);
        comboLightShader.setFloat((uniformPrefix + ".linear").c_str(), 0.09f);
        comboLightShader.setFloat((uniformPrefix + ".quadratic").c_str(), 0.032f);

        // Light properties
        comboLightShader.setVec3((uniformPrefix + ".ambient").c_str(), 0.2f, 0.2f, 0.2f);
        comboLightShader.setVec3((uniformPrefix + ".diffuse").c_str(), 0.5f, 0.5f, 0.5f);
        comboLightShader.setVec3((uniformPrefix + ".specular").c_str(), 1.0f, 1.0f, 1.0f);
        index++;
    }

    // Spotlight
    // Calculation Specifics
    comboLightShader.setVec3("spotLight.position", camera.getCameraPosition());
    comboLightShader.setVec3("spotLight.direction", camera.getCameraFront());
    comboLightShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    comboLightShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));

    // Light properties
    comboLightShader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
    comboLightShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
    comboLightShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

    comboLightShader.setFloat("spotLight.constant", 1.0f);
    comboLightShader.setFloat("spotLight.linear", 0.09f);
    comboLightShader.setFloat("spotLight.quadratic", 0.032f);

    for (const auto& position : cubePositions) {
        const glm::mat4 objectModel = Transform()
                                      .setPosition(position)
                                      .getTransform();

        comboLightShader.setMat4("model", objectModel);
        cubeMesh.render();
    }
}