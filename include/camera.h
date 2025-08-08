/**
 * @file camera.h
 * @brief Base camera class with view/projection matrices and input-based movement.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"

/**
 * @class Camera
 * @brief Abstract camera with mouse look and WASD movement.
 */
class Camera {
public:
    virtual ~Camera() = default;

    /**
     * @brief Generate view matrix for world-to-camera transform.
     * @return View matrix using lookAt
     */
    glm::mat4 getViewMatrix() const {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    /**
     * @brief Generate perspective projection matrix.
     * @return Projection matrix with current FOV and SVGA aspect ratio
     */
    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), static_cast<float>(SVGA_WIDTH) / static_cast<float>(SVGA_HEIGHT),
                                0.1f, 100.0f);
    }

    /**
     * @brief Update camera from input state.
     * @param deltaTime Frame time for movement scaling
     * @note Processes WASD, mouse look, and scroll FOV
     */
    virtual void updateCameraPositions(const float deltaTime) {
        const float cameraSpeed = 2.5f * deltaTime;

        // WASD movement
        if (InputHandler::inputState.w) {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (InputHandler::inputState.s) {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (InputHandler::inputState.a) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (InputHandler::inputState.d) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        // Mouse look
        yaw += InputHandler::inputState.MouseXOffset;
        pitch += InputHandler::inputState.MouseYOffset;

        // Clamp pitch to prevent flipping
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        // Convert yaw/pitch to direction vector
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);

        // Scroll FOV adjustment
        fov -= static_cast<float>(InputHandler::inputState.MouseScrollYOffset) * InputHandler::inputState.sensitivity;
        if (fov < 65.0f) fov = 65.0f;
        if (fov > 90.0f) fov = 90.0f;
    }

protected:
    float fov = 70.0f; ///< Field of view (65°-90°)
    float yaw = -90.0f; ///< Horizontal rotation (starts facing -Z)
    float pitch = 0.0f; ///< Vertical rotation (clamped ±89°)

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); ///< World position
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); ///< Forward direction
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); ///< Up direction
};
