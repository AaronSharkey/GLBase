/**
 * @file transform.h
 * @brief 3D transformation class supporting fluent interface for chaining position, rotation, and scale operations.
 *        Provides method chaining capabilities for building transformation matrices in TRS order.
 *
 * @author Aaron Sharkey
 * @date 2025-07-20
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief 3D transformation class with fluent interface for method chaining.
 *
 * Manages position, rotation, and scale components to generate transformation matrices.
 * All setter methods return *this to enable method chaining.
 *
 * @example
 * @code
 * glm::mat4 matrix = Transform()
 *     .setPosition(glm::vec3(1.0f, 2.0f, 0.0f))
 *     .setRotation(glm::vec3(0.0f, glm::radians(45.0f), 0.0f))
 *     .setScale(glm::vec3(2.0f, 2.0f, 2.0f))
 *     .getTransform();
 * @endcode
 */
class Transform {
public:
    /**
     * @brief Sets position component for method chaining.
     * @param pos Position vector in 3D space
     * @return Transform& Reference to this object for chaining
     */
    Transform &setPosition(const glm::vec3 &pos) {
        position = pos;
        return *this;
    }

    [[nodiscard]] glm::vec3 getPosition() const {
        return position;
    }

    /**
     * @brief Sets rotation component for method chaining.
     * @param rot Rotation angles in radians (x, y, z axes)
     * @return Transform& Reference to this object for chaining
     * @note Rotations applied in XYZ order
     */
    Transform &setRotation(const glm::vec3 &rot) {
        rotation = rot;
        return *this;
    }

    /**
     * @brief Sets scale component for method chaining.
     * @param sca Scale factors for each axis (x, y, z)
     * @return Transform& Reference to this object for chaining
     * @warning Negative values flip object, zero values collapse axis
     */
    Transform &setScale(const glm::vec3 &sca) {
        scale = sca;
        return *this;
    }

    /**
     * @brief Generates final 4x4 transformation matrix in TRS order.
     * @return glm::mat4 Computed transformation matrix for OpenGL shaders
     * @note Applies: Translate → Rotate(X,Y,Z) → Scale
     */
    [[nodiscard]] glm::mat4 getTransform() const {
        auto result = glm::mat4(1.0f); // Reset identity matrix
        result = glm::translate(result, position);
        result = glm::rotate(result, rotation.x, glm::vec3(1, 0, 0));
        result = glm::rotate(result, rotation.y, glm::vec3(0, 1, 0));
        result = glm::rotate(result, rotation.z, glm::vec3(0, 0, 1));
        result = glm::scale(result, scale);
        return result;
    }

private:
    glm::vec3 position{0.0f}; ///< Position in 3D space (x, y, z coordinates)
    glm::vec3 rotation{0.0f}; ///< Rotation angles in radians around each axis (x, y, z)
    glm::vec3 scale{1.0f}; ///< Scale factors along each axis (x, y, z)
};
