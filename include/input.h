/**
 * @file input.h
 * @brief Input management with edge-triggered toggles and mode state tracking.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once

#include <tuple>
#include <map>
#include <set>
#include <glfw/glfw3.h>

/** @brief Mode name -> enabled state */
typedef std::map<std::string, bool> ModeSelection;

/** @brief (previous state, linked mode name) */
typedef std::tuple<bool, std::string> KeyToggleState;

/** @brief GLFW key code -> toggle state */
typedef std::map<int, KeyToggleState> KeyToggleStates;

/**
 * @struct InputState
 * @brief Manages keyboard, mouse, and toggle key states.
 */
struct InputState {
    // Movement Keys
    bool w, a, s, d; ///< WASD movement states
    bool escape; ///< Escape key state

    // Toggle Keys
    KeyToggleStates keyToggleStates = {{GLFW_KEY_X, KeyToggleState{false, "debug-mode"}}};
    ModeSelection modes = {{"debug-mode", false}};

    // Mouse Movement
    bool firstMouse = true;
    double mouseX, mouseY;
    float mouseLastX = SVGA_WIDTH / 2.0f;
    float mouseLastY = SVGA_HEIGHT / 2.0f;
    float MouseXOffset, MouseYOffset;

    double MouseScrollXOffset, MouseScrollYOffset = 0.0f;
    float sensitivity = 0.1f;

    /**
     * @brief Updates all input states for current frame.
     * @param window GLFW window handle
     * @note Uses edge detection for toggles, inverts Y for OpenGL
     */
    void processInput(GLFWwindow *window) {
        // Movement Keys
        w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
        escape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

        // Toggle Keys - edge detection prevents rapid switching
        for (auto &[key, value]: keyToggleStates) {
            auto &[previousState, linkedMode] = value;
            const bool currentPressed = (glfwGetKey(window, key) == GLFW_PRESS);
            if (currentPressed && !previousState) {
                modes[linkedMode] = !modes[linkedMode];
            }
            previousState = currentPressed;
        }

        // Mouse Movement
        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (firstMouse) {
            mouseLastX = mouseX;
            mouseLastY = mouseY;
            firstMouse = false;
        }

        const float xOffset = static_cast<float>(mouseX) - mouseLastX;
        const float yOffset = mouseLastY - static_cast<float>(mouseY); // Invert Y for OpenGL
        mouseLastX = mouseX;
        mouseLastY = mouseY;
        MouseXOffset = xOffset * sensitivity;
        MouseYOffset = yOffset * sensitivity;
    }
};

/**
 * @class InputHandler
 * @brief Static interface for input management and GLFW integration.
 */
class InputHandler {
public:
    inline static InputState inputState{};

    /**
     * @brief Process input for current frame.
     * @param window GLFW window handle
     */
    static void processInput(GLFWwindow *window) {
        inputState.processInput(window);
    }

    /**
     * @brief Set GLFW input mode (cursor visibility, etc.).
     */
    static void setInputMode(GLFWwindow *window, const GLint mode, const GLint value) {
        glfwSetInputMode(window, mode, value);
    }

    /**
     * @brief Register scroll callback with GLFW.
     */
    static void setScrollCallBack(GLFWwindow *window, GLFWscrollfun callback) {
        glfwSetScrollCallback(window, callback = scrollCallback);
    }

    /**
     * @brief Get current state of application mode.
     * @param mode Mode name to query
     * @return Mode state, false if mode doesn't exist
     */
    static bool getModeState(const std::string &mode) {
        if (inputState.modes.contains(mode)) {
            return inputState.modes.at(mode);
        }
        return false;
    }

    /**
     * @brief GLFW scroll callback - updates scroll offset values.
     */
    static void scrollCallback(GLFWwindow *window, const double xOffset, const double yOffset) {
        inputState.MouseScrollXOffset = xOffset;
        inputState.MouseScrollYOffset = yOffset;
    }
};
