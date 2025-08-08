/**
 * @file main.cpp
 * @brief OpenGL application entry point with example selection and debug UI.
 *
 * @author Aaron Sharkey
 * @date 2025-07-17
 */

#include <map>
#include <ranges>

#include "window.h"
#include "input.h"
#include "gui/imgui-manager.h"
#include "examples/spinning-block.h"
#include "examples/textured-block.h"
#include "examples/camera-with-movement.h"
#include "examples/three-dimensional-projection.h"
#include "examples/time-based-dynamic-uniform-animation.h"

/**
 * @brief Application entry point with render loop and example selection.
 * @return 0 on successful execution
 */
int main() {
    // -------------------------------------------------------------------------
    // Initialize Window and Examples
    // -------------------------------------------------------------------------
    Window basicWindow;

    TimeBasedDynamicUniformAnimation basicExample;
    TexturedBlock texturedBlockExample;
    SpinningBlock spinningBlockExample;
    ThreeDimensionalProjection threeDimensionalProjectionExample;
    CameraWithMovement cameraExample;

    // Map example names to instances for selection
    std::map<std::string, Example *> examples = {
        {"basic", &basicExample},
        {"textured-block", &texturedBlockExample},
        {"spinning-block", &spinningBlockExample},
        {"three-dimension-projection", &threeDimensionalProjectionExample},
        {"camera-with-movement", &cameraExample}
    };

    // Extract example names for UI dropdown
    std::vector<std::string> exampleNames;
    exampleNames.reserve(examples.size());
    for (const auto &key: examples | std::views::keys) {
        exampleNames.push_back(key);
    }

    GLFWwindow *window = basicWindow.getWindow();
    ImGuiManager guiManager(window);

    InputHandler::setScrollCallBack(window, &InputHandler::scrollCallback);

    // -------------------------------------------------------------------------
    // Main Render Loop
    // -------------------------------------------------------------------------
    while (!basicWindow.getShouldClose()) {

        InputHandler::processInput(window);

        // Exit on escape key
        if (InputHandler::inputState.escape) {
            basicWindow.setShouldClose();
        }

        ImGuiManager::newFrame();

        // Toggle between UI mode and camera mode
        if (InputHandler::getModeState("debug-mode")) {
            InputHandler::setInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            guiManager.renderExampleSelector(exampleNames);
        } else {
            InputHandler::setInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        float deltaTime = basicWindow.getDeltaTime();

        Window::clear();

        // Render selected example
        examples[guiManager.selectedExample]->render(deltaTime);

        ImGuiManager::render();
        basicWindow.swapBuffers();
        Window::pollEvents();
    }

    // -------------------------------------------------------------------------
    // Cleanup
    // -------------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
