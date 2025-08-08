/**
 * @file fps-camera.h
 * @brief FPS camera with ground-locked movement and debug mode support.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once

#include "camera.h"

/**
 * @class FPSCamera
 * @brief FPS camera with XZ-plane movement and debug mode integration.
 * @note Final class - movement projected to ground plane, disabled in debug mode
 */
class FPSCamera final : public Camera {
public:
    /**
     * @brief Update camera with FPS-specific movement constraints.
     * @param deltaTime Frame time for movement scaling
     * @note Movement uses XZ projection (no flying), disabled in debug mode
     */
    void updateCameraPositions(float deltaTime) override {
        // Disable camera in debug mode for UI interaction
        if (InputHandler::getModeState("debug-mode")) {
            return;
        }

        const float cameraSpeed = 2.5f * deltaTime;

        // Mouse look (same as base)
        yaw += InputHandler::inputState.MouseXOffset;
        pitch += InputHandler::inputState.MouseYOffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);

        // Ground-locked movement - project to XZ plane
        const auto projectedCameraVector = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));

        if (InputHandler::inputState.w) {
            cameraPos += cameraSpeed * projectedCameraVector;
        }
        if (InputHandler::inputState.s) {
            cameraPos -= cameraSpeed * projectedCameraVector;
        }
        if (InputHandler::inputState.a) {
            cameraPos -= glm::normalize(glm::cross(projectedCameraVector, cameraUp)) * cameraSpeed;
        }
        if (InputHandler::inputState.d) {
            cameraPos += glm::normalize(glm::cross(projectedCameraVector, cameraUp)) * cameraSpeed;
        }

        // Scroll FOV
        fov -= static_cast<float>(InputHandler::inputState.MouseScrollYOffset) * InputHandler::inputState.sensitivity;
        if (fov < 65.0f) fov = 65.0f;
        if (fov > 90.0f) fov = 90.0f;
    };
};
