/**
 * @file time-based-dynamic-uniform-animation.h
 * @brief Triangle with time-based color animation using shader uniforms.
 *
 * @author Aaron Sharkey
 * @date 2025-07-17
 */

#pragma once
#include "example.h"
#include "simple-mesh.h"
#include "shader.h"
#include <GLFW/glfw3.h>

/**
 * @class TimeBasedDynamicUniformAnimation
 * @brief Triangle with continuously changing colors calculated from time.
 * @note Colors cycle every 2π seconds using trigonometric functions
 */
class TimeBasedDynamicUniformAnimation final : public Example {
public:
    TimeBasedDynamicUniformAnimation() {
        triangleMesh.addPositionOnlyAttributes();
    };

    void render(float deltaTime) override;

private:
    /**
     * @brief Triangle vertex data in normalized device coordinates.
     * @note Simple triangle centered in viewport
     */
    const float vertices[9] = {
         0.0f,  0.5f, 0.0f,  // Top vertex
         0.5f, -0.5f, 0.0f,  // Bottom right
        -0.5f, -0.5f, 0.0f   // Bottom left
    };

    const GLuint indices[3] = {0, 1, 2}; ///< Triangle indices (CCW winding)

    SimpleMesh triangleMesh{vertices, indices, std::size(vertices), std::size(indices)};
    Shader timeAnimationShader{
        "shaders/examples/time-based-dynamic-uniform-animation/time-based-dynamic-uniform-animation.vert",
        "shaders/examples/time-based-dynamic-uniform-animation/time-based-dynamic-uniform-animation.frag"
    };
};

/**
 * @brief Render triangle with time-based color animation.
 * @param deltaTime Frame time (unused, uses glfwGetTime for color calculation)
 * @note Colors: R=cos(t)/2+0.5, G=sin(t)/2+0.5, B=(sin(t)+cos(t))/4+0.5
 */
inline void TimeBasedDynamicUniformAnimation::render(float deltaTime) {
    timeAnimationShader.use();

    // Calculate time-based color components using trigonometric functions
    const double timeValue = glfwGetTime();
    const auto redValue = static_cast<float>(cos(timeValue) / 2.0 + 0.5);
    const auto greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
    const auto blueValue = static_cast<float>((sin(timeValue) + cos(timeValue)) / 4.0 + 0.5);

    // Update shader uniform and render
    timeAnimationShader.setVec4("dynamicColor", redValue, greenValue, blueValue, 1.0f);
    triangleMesh.render();
}
