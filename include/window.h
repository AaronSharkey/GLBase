/**
 * @file window.h
 * @brief GLFW window management with OpenGL context setup.
 *
 * @author Aaron Sharkey
 * @date 2025-07-16
 */

#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// -----------------------------------------------------------------------------
// Common Screen Resolution Macros
// -----------------------------------------------------------------------------

#define SVGA_WIDTH  800
#define HD_WIDTH    1280
#define FHD_WIDTH   1920
#define QHD_WIDTH   2560
#define UHD_WIDTH   3840

#define SVGA_HEIGHT  600
#define HD_HEIGHT    720
#define FHD_HEIGHT  1080
#define QHD_HEIGHT  1440
#define UHD_HEIGHT  2160

// -----------------------------------------------------------------------------
// Default Window Configuration
// -----------------------------------------------------------------------------

static constexpr auto DEFAULT_TITLE = "GLBase DEV";
inline auto CLEAR_COLOR = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

// -----------------------------------------------------------------------------
// Window Class
// -----------------------------------------------------------------------------

/**
 * @class Window
 * @brief GLFW window with OpenGL 3.3 context and automatic initialization.
 */
class Window {
public:
    /**
     * @brief Create window with HD resolution and default title.
     */
    Window();

    /**
     * @brief Create window with custom dimensions and title.
     * @param width Window width in pixels
     * @param height Window height in pixels
     * @param name Window title
     */
    Window(GLint width, GLint height, const char *name);

    /**
     * @brief GLFW callback to update viewport on window resize.
     */
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

    /**
     * @brief Check if window should close.
     * @return True if close requested
     */
    [[nodiscard]] bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); }

    /**
     * @brief Request window to close.
     */
    void setShouldClose() const { glfwSetWindowShouldClose(mainWindow, true); }

    /**
     * @brief Swap front/back buffers for double-buffered rendering.
     */
    void swapBuffers() const { glfwSwapBuffers(mainWindow); }

    /**
     * @brief Toggle UI interaction mode.
     */
    void toggleUiMode() { uiMode = !uiMode; };

    /**
     * @brief Poll and process window/input events.
     */
    static void pollEvents() { glfwPollEvents(); }

    /**
     * @brief Get frame delta time and update timer.
     * @return Time since last frame in seconds
     */
    float getDeltaTime() {
        const auto currentTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        return deltaTime;
    };

    /**
     * @brief Get GLFW window handle.
     * @return Pointer to GLFW window
     */
    [[nodiscard]] GLFWwindow *getWindow() const { return mainWindow; }

    /**
     * @brief Clear screen with default color and depth.
     */
    static void clear();

private:
    GLint width = FHD_WIDTH; ///< Window width in pixels
    GLint height = FHD_HEIGHT; ///< Window height in pixels
    const char *name = DEFAULT_TITLE; ///< Window title
    GLFWwindow *mainWindow = nullptr; ///< GLFW window handle
    float lastFrame = 0.0f; ///< Last frame timestamp
    bool uiMode = false; ///< UI interaction mode flag

    /**
     * @brief Initialize GLFW, create window, setup OpenGL context.
     * @return 0 on success, -1 on failure
     */
    int initialise();
};

// -----------------------------------------------------------------------------
// Inline Implementations
// -----------------------------------------------------------------------------

inline Window::Window() {
    initialise();
}

inline Window::Window(const GLint width, const GLint height, const char *name)
    : width(width), height(height), name(name) {
    initialise();
}

inline void Window::frameBufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

inline void Window::clear() {
    glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    glEnable(GL_DEPTH_TEST);

    return 0;
}
