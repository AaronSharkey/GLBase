#include <gtest/gtest.h>
#include <glm/gtc/constants.hpp>
#include "transform.h"

class TransformTest : public ::testing::Test {
protected:
    Transform transform;

    // Helper function to check if matrices are approximately equal
    static void expectMatrixNear(const glm::mat4& actual, const glm::mat4& expected, const float tolerance = 1e-5f) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                EXPECT_NEAR(actual[i][j], expected[i][j], tolerance)
                    << "Matrix element [" << i << "][" << j << "] differs";
            }
        }
    }
};

TEST_F(TransformTest, DefaultTransformIsIdentity) {
    glm::mat4 matrix = transform.getTransform();
    glm::mat4 identity = glm::mat4(1.0f);

    expectMatrixNear(matrix, identity);
}

TEST_F(TransformTest, PositionOnlyTransform) {
    glm::vec3 position(2.0f, 3.0f, 4.0f);
    glm::mat4 matrix = transform.setPosition(position).getTransform();

    // Check translation components
    EXPECT_FLOAT_EQ(matrix[3][0], 2.0f);  // X translation
    EXPECT_FLOAT_EQ(matrix[3][1], 3.0f);  // Y translation
    EXPECT_FLOAT_EQ(matrix[3][2], 4.0f);  // Z translation

    // Rotation and scale should remain identity
    EXPECT_FLOAT_EQ(matrix[0][0], 1.0f);  // X scale
    EXPECT_FLOAT_EQ(matrix[1][1], 1.0f);  // Y scale
    EXPECT_FLOAT_EQ(matrix[2][2], 1.0f);  // Z scale
}

TEST_F(TransformTest, ScaleOnlyTransform) {
    glm::vec3 scale(2.0f, 0.5f, 3.0f);
    glm::mat4 matrix = transform.setScale(scale).getTransform();

    // Check scale components (diagonal elements)
    EXPECT_FLOAT_EQ(matrix[0][0], 2.0f);   // X scale
    EXPECT_FLOAT_EQ(matrix[1][1], 0.5f);   // Y scale
    EXPECT_FLOAT_EQ(matrix[2][2], 3.0f);   // Z scale

    // Translation should be zero
    EXPECT_FLOAT_EQ(matrix[3][0], 0.0f);
    EXPECT_FLOAT_EQ(matrix[3][1], 0.0f);
    EXPECT_FLOAT_EQ(matrix[3][2], 0.0f);
}

TEST_F(TransformTest, RotationAroundXAxis) {
    float angle = glm::half_pi<float>();  // 90 degrees
    glm::mat4 matrix = transform.setRotation(glm::vec3(angle, 0.0f, 0.0f)).getTransform();

    // 90° rotation around X-axis: Y becomes -Z, Z becomes Y
    EXPECT_NEAR(matrix[1][1], 0.0f, 1e-5f);   // cos(90°) ≈ 0
    EXPECT_NEAR(matrix[1][2], 1.0f, 1e-5f);   // sin(90°) = 1
    EXPECT_NEAR(matrix[2][1], -1.0f, 1e-5f);  // -sin(90°) = -1
    EXPECT_NEAR(matrix[2][2], 0.0f, 1e-5f);   // cos(90°) ≈ 0
}

TEST_F(TransformTest, RotationAroundYAxis) {
    float angle = glm::half_pi<float>();  // 90 degrees
    glm::mat4 matrix = transform.setRotation(glm::vec3(0.0f, angle, 0.0f)).getTransform();

    // 90° rotation around Y-axis: X becomes Z, Z becomes -X
    EXPECT_NEAR(matrix[0][0], 0.0f, 1e-5f);   // cos(90°) ≈ 0
    EXPECT_NEAR(matrix[0][2], -1.0f, 1e-5f);  // -sin(90°) = -1
    EXPECT_NEAR(matrix[2][0], 1.0f, 1e-5f);   // sin(90°) = 1
    EXPECT_NEAR(matrix[2][2], 0.0f, 1e-5f);   // cos(90°) ≈ 0
}

TEST_F(TransformTest, RotationAroundZAxis) {
    float angle = glm::half_pi<float>();  // 90 degrees
    glm::mat4 matrix = transform.setRotation(glm::vec3(0.0f, 0.0f, angle)).getTransform();

    // 90° rotation around Z-axis: X becomes Y, Y becomes -X
    EXPECT_NEAR(matrix[0][0], 0.0f, 1e-5f);   // cos(90°) ≈ 0
    EXPECT_NEAR(matrix[0][1], 1.0f, 1e-5f);   // sin(90°) = 1
    EXPECT_NEAR(matrix[1][0], -1.0f, 1e-5f);  // -sin(90°) = -1
    EXPECT_NEAR(matrix[1][1], 0.0f, 1e-5f);   // cos(90°) ≈ 0
}

TEST_F(TransformTest, MethodChaining) {
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 rotation(0.0f, 0.0f, glm::half_pi<float>());
    glm::vec3 scale(2.0f, 2.0f, 2.0f);

    // Test that chaining returns correct reference
    Transform& result = transform.setPosition(position)
                                .setRotation(rotation)
                                .setScale(scale);

    // Should return reference to same object
    EXPECT_EQ(&result, &transform);

    glm::mat4 matrix = result.getTransform();

    // Verify final matrix has all transformations applied
    // Translation should be present
    EXPECT_FLOAT_EQ(matrix[3][0], 1.0f);
    EXPECT_FLOAT_EQ(matrix[3][1], 2.0f);
    EXPECT_FLOAT_EQ(matrix[3][2], 3.0f);

    // Scale should be applied (visible in rotated matrix)
    // After 90° Z rotation and 2x scale, the matrix should show scaling
    EXPECT_NEAR(matrix[0][1], 2.0f, 1e-5f);  // Scaled Y component in X column
    EXPECT_NEAR(matrix[1][0], -2.0f, 1e-5f); // Scaled -X component in Y column
}

TEST_F(TransformTest, TRSOrderVerification) {
    // Apply scale, then rotation, then translation in that order
    glm::vec3 position(5.0f, 0.0f, 0.0f);
    glm::vec3 rotation(0.0f, 0.0f, glm::half_pi<float>());  // 90° around Z
    glm::vec3 scale(2.0f, 1.0f, 1.0f);  // Scale X by 2

    glm::mat4 matrix = transform.setPosition(position)
                               .setRotation(rotation)
                               .setScale(scale)
                               .getTransform();

    // With TRS order: Scale first (2x in X), then rotate 90° Z, then translate
    // Original point (1,0,0) becomes:
    // 1. Scale: (2,0,0)
    // 2. Rotate 90° Z: (0,2,0)
    // 3. Translate: (5,2,0)

    glm::vec4 testPoint(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 transformed = matrix * testPoint;

    EXPECT_NEAR(transformed.x, 5.0f, 1e-5f);
    EXPECT_NEAR(transformed.y, 2.0f, 1e-5f);
    EXPECT_NEAR(transformed.z, 0.0f, 1e-5f);
}

TEST_F(TransformTest, ZeroScale) {
    glm::vec3 zeroScale(0.0f, 0.0f, 0.0f);
    glm::mat4 matrix = transform.setScale(zeroScale).getTransform();

    // Zero scale should collapse all axes
    EXPECT_FLOAT_EQ(matrix[0][0], 0.0f);
    EXPECT_FLOAT_EQ(matrix[1][1], 0.0f);
    EXPECT_FLOAT_EQ(matrix[2][2], 0.0f);
}

TEST_F(TransformTest, NegativeScale) {
    glm::vec3 negativeScale(-1.0f, -1.0f, -1.0f);
    glm::mat4 matrix = transform.setScale(negativeScale).getTransform();

    // Negative scale should flip all axes
    EXPECT_FLOAT_EQ(matrix[0][0], -1.0f);
    EXPECT_FLOAT_EQ(matrix[1][1], -1.0f);
    EXPECT_FLOAT_EQ(matrix[2][2], -1.0f);
}

TEST_F(TransformTest, MultipleTransformCalls) {
    // Test that multiple calls override previous values
    transform.setPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    transform.setPosition(glm::vec3(4.0f, 5.0f, 6.0f));

    glm::mat4 matrix = transform.getTransform();

    // Should have the latest position values
    EXPECT_FLOAT_EQ(matrix[3][0], 4.0f);
    EXPECT_FLOAT_EQ(matrix[3][1], 5.0f);
    EXPECT_FLOAT_EQ(matrix[3][2], 6.0f);
}
