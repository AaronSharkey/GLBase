/**
 * @file main.cpp
 * @brief Entry point for OpenGL base application using custom Shader and Mesh classes.
 *        Creates a window, sets up GL context, loads shaders, and renders a quad.
 */

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "mesh.h"

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------

static constexpr unsigned int WIDTH = 800;
static constexpr unsigned int HEIGHT = 600;
static const char* TITLE = "GLBase DEV";

// -----------------------------------------------------------------------------
// Vertex Data
// -----------------------------------------------------------------------------

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};

const GLint indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
};

// -----------------------------------------------------------------------------
// Input Handling
// -----------------------------------------------------------------------------

/**
 * @brief Handles input per-frame. Closes the window when ESC is pressed.
 */
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/**
 * @brief Callback for window resize. Updates the OpenGL viewport.
 */
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL x.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile only

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "ERROR::GLAD::FAILED_TO_INITIALIZE" << std::endl;
        return -1;
    }

    // Set initial viewport and register resize callback
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // -------------------------------------------------------------------------
    // Resource Initialization
    // -------------------------------------------------------------------------

    const Shader basicShader("shaders/basic/basic.vert", "shaders/basic/basic.frag");
    const Mesh basicMesh(vertices, indices, std::size(vertices), std::size(indices), 3, true); // wireframe enabled

    // -------------------------------------------------------------------------
    // Render Loop
    // -------------------------------------------------------------------------

    while (!glfwWindowShouldClose(window)) {
        // Handle per-frame input
        processInput(window);

        // Clear screen with a dark teal-ish color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader and draw mesh
        basicShader.use();
        basicMesh.render();

        // Swap front and back buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // -------------------------------------------------------------------------
    // Cleanup
    // -------------------------------------------------------------------------

    glfwTerminate();
    return 0;
}
