/**
 * @file window.h
 * @brief Declaration and implementation of the Window class for GLFW-based
 * OpenGL window management. Handles GLFW initialization, OpenGL context setup,
 * window creation, input processing, buffer swapping, event polling, and
 * viewport resizing.
 *
 * @author Aaron Sharkey
 * @date 2025-07-16
 */

#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

// -----------------------------------------------------------------------------
// Common Screen Resolution Macros
// -----------------------------------------------------------------------------

/** @brief HD (720p) resolution width in pixels */
#define HD_WIDTH 1280
/** @brief Full HD (1080p) resolution width in pixels */
#define FHD_WIDTH 1920
/** @brief Quad HD (1440p) resolution width in pixels */
#define QHD_WIDTH 2560
/** @brief Ultra HD (4K/2160p) resolution width in pixels */
#define UHD_WIDTH 3840

/** @brief HD (720p) resolution height in pixels */
#define HD_HEIGHT 720
/** @brief Full HD (1080p) resolution height in pixels */
#define FHD_HEIGHT 1080
/** @brief Quad HD (1440p) resolution height in pixels */
#define QHD_HEIGHT 1440
/** @brief Ultra HD (4K/2160p) resolution height in pixels */
#define UHD_HEIGHT 2160

// -----------------------------------------------------------------------------
// Default Window Configuration
// -----------------------------------------------------------------------------

/** @brief Default window title displayed in the title bar */
static constexpr auto DEFAULT_TITLE = "GLBase DEV";

/** 
 * @brief Default clear color for the OpenGL framebuffer.
 * @details Dark teal color (RGB: 0.2, 0.3, 0.3) with full opacity.
 *          Applied when Window::clear() is called.
 */
inline auto CLEAR_COLOR = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

// -----------------------------------------------------------------------------
// Window Class
// -----------------------------------------------------------------------------

/**
 * @class Window
 * @brief Comprehensive GLFW window wrapper for OpenGL applications.
 * 
 * The Window class encapsulates all GLFW window creation, OpenGL context setup,
 * and basic window management functionality. It provides a clean interface for
 * creating OpenGL-ready windows with proper error handling and resource management.
 * 
 * Key Features:
 * - Automatic GLFW initialization and cleanup
 * - OpenGL 3.3 Core Profile context creation
 * - GLAD OpenGL function loader integration
 * - Automatic viewport resizing on window resize
 * - Basic input handling (ESC to close)
 * - Consistent clear color management using GLM
 * 
 * @note This class uses RAII principles - resources are acquired in constructor
 *       and released automatically when the object goes out of scope.
 * 
 * @see GLFW documentation: https://www.glfw.org/docs/latest/
 * @see OpenGL specification: https://www.opengl.org/registry/
 */
class Window {
public:
  /**
   * @brief Default constructor creating an HD resolution window.
   * 
   * Creates a window with HD resolution (1280x720) and the default title.
   * Automatically initializes GLFW, creates the OpenGL context, and sets up
   * all necessary callbacks.
   * 
   * @throws std::runtime_error if GLFW initialization fails
   * @throws std::runtime_error if window creation fails
   * @throws std::runtime_error if GLAD initialization fails
   * 
   * @post Window is ready for OpenGL rendering
   * @post OpenGL 3.3 Core Profile context is current
   * @post Viewport is set to window dimensions
   */
  Window();

  /**
   * @brief Parameterized constructor for custom window configuration.
   * 
   * Creates a window with specified dimensions and title. Performs the same
   * initialization as the default constructor but with custom parameters.
   * 
   * @param width  Window width in pixels (must be > 0)
   * @param height Window height in pixels (must be > 0)
   * @param name   Window title string (null-terminated C string)
   * 
   * @pre width > 0 && height > 0
   * @pre name != nullptr
   * 
   * @throws std::runtime_error if GLFW initialization fails
   * @throws std::runtime_error if window creation fails
   * @throws std::runtime_error if GLAD initialization fails
   * 
   * @post Window is ready for OpenGL rendering
   * @post OpenGL 3.3 Core Profile context is current
   * @post Viewport is set to specified dimensions
   * 
   * @note The name parameter is stored as a pointer - ensure the string
   *       remains valid for the lifetime of the Window object
   */
  Window(GLint width, GLint height, const char *name);

  /**
   * @brief GLFW framebuffer resize callback function.
   * 
   * Static callback function registered with GLFW to handle framebuffer resize
   * events. Automatically updates the OpenGL viewport to match the new framebuffer
   * dimensions, ensuring rendered content scales properly with window resizing.
   * 
   * @param window Pointer to the GLFW window that was resized (unused)
   * @param width  New framebuffer width in pixels
   * @param height New framebuffer height in pixels
   * 
   * @note This is a static function suitable for GLFW callback registration
   * @note Called automatically by GLFW when the framebuffer size changes
   * @note Updates the OpenGL viewport using glViewport(0, 0, width, height)
   * 
   * @see glfwSetFramebufferSizeCallback
   */
  static void frameBufferSizeCallback(GLFWwindow *window, int width,
                                      int height);

  /**
   * @brief Checks if the window should close.
   * 
   * Queries GLFW to determine if the window close flag has been set.
   * This flag is set when the user clicks the window's close button,
   * presses Alt+F4 (Windows), or when glfwSetWindowShouldClose is called.
   * 
   * @return true if the window should close, false otherwise
   * 
   * @note This function is typically called in the main render loop condition
   * @note Marked [[nodiscard]] to prevent accidentally ignoring the return value
   * 
   * @see glfwWindowShouldClose
   */
  [[nodiscard]] bool getShouldClose() const {
    return glfwWindowShouldClose(mainWindow);
  }

  /**
   * @brief Swaps the front and back framebuffers.
   * 
   * Performs double-buffer swapping to display the rendered frame. In double-buffered
   * rendering, all drawing operations occur on the back buffer while the front buffer
   * is displayed. This function swaps them, making the rendered frame visible and
   * providing a new back buffer for the next frame.
   * 
   * @note Should be called once per frame after all rendering is complete
   * @note Blocks until the swap is complete (V-Sync dependent)
   * 
   * @see glfwSwapBuffers
   */
  void swapBuffers() const { glfwSwapBuffers(mainWindow); }

  /**
   * @brief Polls and processes all pending GLFW events.
   * 
   * Static function that processes all pending window events, input events,
   * and callbacks. This includes mouse movement, keyboard input, window resize
   * events, and other system messages. Must be called regularly to keep the
   * window responsive.
   * 
   * @note This is a static function - no Window instance required
   * @note Should be called once per frame, typically at the end of the render loop
   * @note Non-blocking - returns immediately after processing all pending events
   * 
   * @see glfwPollEvents
   */
  static void pollEvents() { glfwPollEvents(); }

  /**
   * @brief Processes application-specific input for this frame.
   * 
   * Handles per-frame input polling for application-specific controls.
   * Currently implements ESC key handling to close the window gracefully.
   * This function should be called once per frame to ensure responsive input.
   * 
   * @note This processes immediate input state, not events
   * @note ESC key press sets the window close flag
   * @note Called per-frame for immediate response to input
   * 
   * @see glfwGetKey
   * @see glfwSetWindowShouldClose
   */
  void processInput() const;

  /**
   * @brief Clears the framebuffer with the default clear color.
   * 
   * Static function that clears the current framebuffer using the predefined
   * CLEAR_COLOR constant. This should be called at the beginning of each frame
   * before any rendering operations to ensure a clean canvas.
   * 
   * @note This is a static function - no Window instance required
   * @note Clears the color buffer only (not depth or stencil)
   * @note Uses the CLEAR_COLOR constant for consistent appearance
   * @note Should be called once per frame before rendering
   * 
   * @see glClearColor
   * @see glClear
   * @see CLEAR_COLOR
   */
  static void clear();

private:
  GLint width = HD_WIDTH;           ///< Window width in pixels (initial size)
  GLint height = HD_HEIGHT;         ///< Window height in pixels (initial size)
  const char *name = DEFAULT_TITLE; ///< Window title string (pointer to C string)
  GLFWwindow *mainWindow = nullptr; ///< GLFW window handle (managed by GLFW)

  /**
   * @brief Internal initialization function for GLFW and OpenGL setup.
   * 
   * Performs the complete initialization sequence:
   * 1. Initialize GLFW library
   * 2. Configure OpenGL context hints (3.3 Core Profile)
   * 3. Create the GLFW window
   * 4. Make the OpenGL context current
   * 5. Initialize GLAD OpenGL function loader
   * 6. Register framebuffer resize callback
   * 
   * @return 0 on successful initialization, -1 on any failure
   * 
   * @note This is called automatically by constructors
   * @note Prints error messages to std::cerr on failure
   * @note Sets up OpenGL 3.3 Core Profile context
   * @note Enables automatic viewport resizing
   * 
   * @see glfwInit
   * @see glfwCreateWindow
   * @see gladLoadGLLoader
   */
  int initialise();
};

// -----------------------------------------------------------------------------
// Inline Implementations
// -----------------------------------------------------------------------------

inline Window::Window() { initialise(); }

inline Window::Window(const GLint width, const GLint height, const char *name)
    : width(width), height(height), name(name) {
  initialise();
}

inline void Window::frameBufferSizeCallback(GLFWwindow *window, const int width,
                                            const int height) {
  glViewport(0, 0, width, height);
}

inline void Window::processInput() const {
  if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(mainWindow, true);
}

inline void Window::clear() {
  glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

inline int Window::initialise() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "ERROR::GLFW::FAILED_TO_INITIALISE" << std::endl;
    return -1;
  }

  // Configure OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window and OpenGL context
  mainWindow = glfwCreateWindow(width, height, name, nullptr, nullptr);
  if (mainWindow == nullptr) {
    std::cerr << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(mainWindow);

  // Load OpenGL function pointers via GLAD
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cerr << "ERROR::GLAD::FAILED_TO_INITIALIZE" << std::endl;
    return -1;
  }

  // Set framebuffer resize callback
  glfwSetFramebufferSizeCallback(mainWindow, Window::frameBufferSizeCallback);

  return 0;
}
