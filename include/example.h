/**
* @file example.h
 * @brief Abstract base class for modular OpenGL rendering demonstrations.
 *        Provides a common interface for graphics programming examples that can
 *        be easily switched and managed in the main render loop.
 *
 * @author Aaron Sharkey
 * @date 2025-07-17
 */

#pragma once

/**
 * @class Example
 * @brief Abstract base for self-contained graphics programming demonstrations.
 *
 * Each concrete implementation showcases a specific OpenGL technique or effect.
 * Examples manage their own resources (shaders, meshes, textures) and provide
 * a consistent interface for rendering and state management.
 *
 * Typical implementation pattern:
 * ```cpp
 * class MyExample final : public Example {
 * private:
 *     Shader myShader{"vertex.vert", "fragment.frag"};
 *     Mesh myMesh{vertices, indices, ...};
 *
 * public:
 *     void render() override {
 *         myShader.use();
 *         // Set uniforms, render geometry
 *         myMesh.render();
 *     }
 * };
 * ```
 */
class Example {
public:
    /**
     * @brief Performs all rendering for this example.
     *        Must be implemented by derived classes to define their specific
     *        rendering behavior and visual effects.
     */
    virtual void render(float deltaTime) = 0;

    /**
     * @brief Virtual destructor ensuring proper polymorphic cleanup.
     *        Guarantees derived class destructors are called when deleting
     *        through base class pointers.
     */
    virtual ~Example() = default;
};
