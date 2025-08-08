/**
 * @file imgui-manager.h
 * @brief IMGUI manager for example selection and debug UI.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <string>

/**
 * @class ImGuiManager
 * @brief IMGUI wrapper for example selection and debug windows.
 */
class ImGuiManager {
public:
    /**
     * @brief Initialize IMGUI with GLFW window.
     * @param window GLFW window handle
     * @param style Use dark theme if true, light if false
     * @param glslVersion OpenGL shader version string
     */
    explicit ImGuiManager(GLFWwindow *window, bool style = true, const char *glslVersion = "#version 330");

    /**
     * @brief Start IMGUI frame.
     */
    static void newFrame();

    /**
     * @brief Finalize IMGUI rendering.
     */
    static void render();

    /**
     * @brief Render simple test window with demo controls.
     */
    void renderSimpleWindow();

    /**
     * @brief Render example selection dropdown.
     * @param exampleNames List of available example names
     */
    void renderExampleSelector(const std::vector<std::string> &exampleNames);

    /**
     * @brief Toggle window visibility.
     */
    void toggleShowWindow() { showWindow = !showWindow; };

    /**
     * @brief Clean up IMGUI resources.
     */
    ~ImGuiManager() = default;

    std::string selectedExample = "basic"; ///< Currently selected example name

private:
    bool style; ///< UI style flag (dark/light)
    GLFWwindow *window; ///< GLFW window handle
    bool showWindow = true; ///< Window visibility flag
    const char *glslVersion; ///< OpenGL shader version
};

inline ImGuiManager::ImGuiManager(GLFWwindow *window, bool style, const char *glslVersion)
    : window(window), style(style), glslVersion(glslVersion) {
    // Setup IMGUI context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Apply theme
    if (style) {
        ImGui::StyleColorsDark();
    } else {
        ImGui::StyleColorsLight();
    }

    // Initialize backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

inline void ImGuiManager::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

inline void ImGuiManager::renderSimpleWindow() {
    if (ImGui::Begin("GLBase Example Selection", &showWindow)) {
        ImGui::Text("Hello, World!");
        static float f = 0.0f;
        ImGui::SliderFloat("Float", &f, 0.0f, 1.0f);

        if (ImGui::Button("Click Me!")) {
            // Button was clicked
        }
    }
    ImGui::End();
}

inline void ImGuiManager::renderExampleSelector(const std::vector<std::string> &exampleNames) {
    if (ImGui::Begin("GLBase Example Selection", &showWindow)) {
        // Example selection dropdown
        if (ImGui::BeginCombo("Select Example", selectedExample.c_str())) {
            for (const auto &exampleName: exampleNames) {
                const bool isSelected = (selectedExample == exampleName);

                if (ImGui::Selectable(exampleName.c_str(), isSelected)) {
                    selectedExample = exampleName;
                }

                // Highlight current selection
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Display current selection
        ImGui::Text("Current Example: %s", selectedExample.c_str());
    }
    ImGui::End();
}

inline void ImGuiManager::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
