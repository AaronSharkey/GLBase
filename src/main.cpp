/**
 * @file main.cpp
 * @brief Main entry point for the GLBase OpenGL demonstration application.
 * 
 * This file contains the main application loop for the GLBase project, which
 * serves as a foundation for OpenGL graphics programming demonstrations. The
 * application creates a window, initializes OpenGL context, and runs examples
 * that showcase various graphics programming techniques.
 * 
 * Application Architecture:
 * - **Window Management**: GLFW-based window with OpenGL 3.3 Core Profile
 * - **Resource Management**: RAII-based automatic cleanup of OpenGL resources
 * - **Example System**: Polymorphic example classes for modular demonstrations
 * - **Render Loop**: Standard game loop with input, update, and render phases
 * 
 * Current Configuration:
 * - Displays TimeBasedDynamicUniformAnimation example
 * - HD resolution window (1280x720) with dark teal clear color
 * - ESC key exits the application
 * - V-sync enabled through GLFW buffer swapping
 * 
 * Build Requirements:
 * - C++20 compiler with standard library support
 * - GLFW 3.3+ for window management
 * - GLAD for OpenGL function loading
 * - GLM for mathematics (vec4 clear color)
 * 
 * @author Aaron Sharkey
 * @date 2025-07-17
 * 
 * @see Window class for window management
 * @see Example class for demonstration interface
 * @see TimeBasedDynamicUniformAnimation for current example
 */

#include "examples/time-based-dynamic-uniform-animation.h" ///< Current example demonstration
#include "window.h"                                        ///< Window management and OpenGL setup

// -----------------------------------------------------------------------------
// Application Entry Point
// -----------------------------------------------------------------------------

/**
 * @brief Main application entry point and render loop.
 * 
 * Initializes the OpenGL application and runs the main render loop until
 * the user requests to exit. Follows standard real-time graphics application
 * structure with initialization, render loop, and cleanup phases.
 * 
 * Execution Flow:
 * 1. **Initialization Phase**:
 *    - Create and configure GLFW window with OpenGL context
 *    - Initialize the current graphics demonstration example
 *    - Verify all resources are properly loaded
 * 
 * 2. **Render Loop Phase**:
 *    - Process user input (ESC to exit)
 *    - Clear framebuffer with consistent background color
 *    - Execute current example's rendering logic
 *    - Present finished frame and poll for events
 *    - Continue until exit condition is met
 * 
 * 3. **Cleanup Phase**:
 *    - Automatic RAII cleanup of C++ objects
 *    - Explicit GLFW termination for clean shutdown
 * 
 * @return 0 on successful execution, non-zero on error
 * 
 * @note Uses RAII principles - most cleanup is automatic
 * @note GLFW requires explicit termination call
 * @note Current example can be changed by modifying the basicExample instantiation
 * 
 * @see Window constructor for OpenGL initialization details
 * @see Example::render() for per-frame rendering contract
 * @see glfwTerminate() for cleanup requirements
 */
int main() {
  // -------------------------------------------------------------------------
  // Resource Initialization Phase
  // -------------------------------------------------------------------------
  
  /** @brief Main application window with OpenGL 3.3 context */
  const Window basicWindow;
  
  /** @brief Current graphics demonstration (easily swappable) */
  TimeBasedDynamicUniformAnimation basicExample;

  // -------------------------------------------------------------------------
  // Main Render Loop Phase
  // -------------------------------------------------------------------------
  while (!basicWindow.getShouldClose()) {
    // Handle per-frame input processing
    basicWindow.processInput();

    // Clear framebuffer with consistent background color
    Window::clear();

    // Execute current example's rendering logic
    basicExample.render();

    // Present completed frame and process system events
    basicWindow.swapBuffers();
    Window::pollEvents();
  }

  // -------------------------------------------------------------------------
  // Cleanup Phase
  // -------------------------------------------------------------------------
  
  // RAII handles automatic cleanup of Window and Example objects
  // Explicit GLFW termination required for complete cleanup
  glfwTerminate();
  
  return 0; // Successful execution
}
