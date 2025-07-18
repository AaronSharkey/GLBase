/**
 * @file example.h
 * @brief Abstract base class for modular OpenGL rendering demonstrations.
 * 
 * This file defines the Example interface that provides a common contract
 * for all graphics programming demonstrations in the GLBase project. Each
 * example encapsulates a specific rendering technique, effect, or OpenGL
 * feature demonstration.
 * 
 * The design enables:
 * - Modular organization of graphics techniques
 * - Easy switching between different rendering examples
 * - Consistent interface for demonstration management
 * - Future integration with UI-based example selection
 * - Educational progression through graphics programming concepts
 * 
 * @author Aaron Sharkey
 * @date 2025-07-17
 * 
 * @see TimeBasedDynamicUniformAnimation for a concrete implementation
 */

#pragma once

/**
 * @class Example
 * @brief Abstract interface for self-contained graphics programming demonstrations.
 * 
 * The Example class serves as a polymorphic base for all rendering demonstrations
 * in the GLBase project. Each concrete implementation showcases a specific OpenGL
 * technique, graphics programming concept, or visual effect.
 * 
 * Architecture Benefits:
 * - **Polymorphic Design**: Enables runtime switching between examples
 * - **Encapsulation**: Each example manages its own resources and state
 * - **Modularity**: Examples can be developed and tested independently
 * - **Extensibility**: New techniques can be added without modifying existing code
 * - **Educational Structure**: Progressive complexity in graphics programming
 * 
 * Implementation Requirements:
 * - Derived classes must implement the pure virtual render() method
 * - Examples should manage their own Shader, Mesh, and texture resources
 * - Rendering should be self-contained within the render() method
 * - Examples should handle their own per-frame logic and state updates
 * 
 * Typical Example Structure:
 * ```cpp
 * class MyExample : public Example {
 * private:
 *     Shader myShader{"vertex.vert", "fragment.frag"};
 *     Mesh myMesh{vertices, indices, ...};
 *     // Other resources...
 * 
 * public:
 *     void render() override {
 *         myShader.use();
 *         // Set uniforms, update state...
 *         myMesh.render();
 *     }
 * };
 * ```
 * 
 * @note This class uses pure virtual functions, making it abstract
 * @note Virtual destructor ensures proper cleanup of derived classes
 * @note Examples should be designed to work with the main render loop
 * 
 * @see TimeBasedDynamicUniformAnimation for a concrete implementation example
 */
class Example {
public:
  /**
   * @brief Pure virtual function that performs all rendering for this example.
   * 
   * This method must be implemented by all derived classes to define their
   * specific rendering behavior. The implementation should contain all logic
   * necessary to render the example's visual content in a single frame.
   * 
   * Typical Implementation Pattern:
   * 1. Activate the appropriate shader program
   * 2. Update and set uniform variables (time, transforms, etc.)
   * 3. Bind and configure any textures or additional resources
   * 4. Render the geometry (meshes, primitives)
   * 5. Handle any per-frame state updates
   * 
   * @pre OpenGL context must be current and valid
   * @pre Window clear operations should be handled by the caller
   * @pre Viewport should be properly configured by the caller
   * 
   * @post All example-specific rendering is complete for this frame
   * @post OpenGL state should be left in a clean condition
   * 
   * @note This is a pure virtual function (= 0), making the class abstract
   * @note Called once per frame from the main render loop
   * @note Should not handle buffer swapping or event polling
   * @note Performance: Keep frame-by-frame operations efficient
   * 
   * @see Window::clear() for framebuffer clearing
   * @see Window::swapBuffers() for display presentation
   * 
   * @example
   * @code
   * void MyExample::render() override {
   *     shader.use();
   *     shader.setUniform4f("color", 1.0f, 0.0f, 0.0f, 1.0f);
   *     mesh.render();
   * }
   * @endcode
   */
  virtual void render() = 0;

  /**
   * @brief Virtual destructor ensuring proper polymorphic cleanup.
   * 
   * Provides a virtual destructor to guarantee that derived class destructors
   * are called correctly when deleting objects through base class pointers.
   * This is essential for proper RAII and resource management in polymorphic
   * class hierarchies.
   * 
   * @note Uses = default to generate the standard destructor behavior
   * @note Virtual keyword ensures derived destructors are called in correct order
   * @note Critical for polymorphic deletion safety
   * 
   * @see Virtual destructor documentation in docs/GLOSSARY.md
   * 
   * @example
   * @code
   * std::unique_ptr<Example> example = std::make_unique<TimeBasedExample>();
   * // Virtual destructor ensures TimeBasedExample::~TimeBasedExample() is called
   * @endcode
   */
  virtual ~Example() = default;
};