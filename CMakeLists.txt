# -----------------------------------------------------------------------------
# CMake configuration for GLBase
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.31)
project(GLBase)

# -----------------------------------------------------------------------------
# 0. General Build Settings
# -----------------------------------------------------------------------------

# Export compile_commands.json (helpful for IDEs like CLion and tools like clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable C++20 with required enforcement
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# 0.5. Google Test Setup
# -----------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# -----------------------------------------------------------------------------
# 0.6. Dear ImGui Setup
# -----------------------------------------------------------------------------

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.91.0  # Latest stable version
)

FetchContent_MakeAvailable(imgui)

# Create ImGui library with OpenGL3 + GLFW backend
add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# ImGui needs GLFW and OpenGL
target_link_libraries(imgui PRIVATE glfw glad)

if (APPLE)
    target_link_libraries(imgui PRIVATE
            "-framework OpenGL"
    )
endif()

# -----------------------------------------------------------------------------
# 0.7. Assimp Setup
# -----------------------------------------------------------------------------

# Configure Assimp build options before fetching
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.4.3 # Latest stable
)

FetchContent_MakeAvailable(assimp)

# -----------------------------------------------------------------------------
# 1. GLAD: OpenGL Function Loader (Built as Static Library)
# -----------------------------------------------------------------------------

add_library(glad STATIC
        ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c
)

target_include_directories(glad PUBLIC
        ${CMAKE_SOURCE_DIR}/libs/glad/include
)

# -----------------------------------------------------------------------------
# 2. GLFW: Windowing Library (Use system install or submodule fallback)
# -----------------------------------------------------------------------------

# macOS: Point to Homebrew if needed
if (APPLE)
    set(CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

# Attempt to find installed GLFW package
find_package(glfw3 3.3 QUIET)

# If not found, fallback to submodule (future support)
if (NOT glfw3_FOUND)
    message(STATUS "GLFW not found globally. Falling back to submodule.")
    # TODO: Add glfw submodule build (uncomment when ready)
    # add_subdirectory(libs/glfw)
endif()

# -----------------------------------------------------------------------------
# 3. Application Sources
# -----------------------------------------------------------------------------

# Collect all .cpp source files from /src directory
file(GLOB APP_SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/libs/stb_image/*.cpp
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

# -----------------------------------------------------------------------------
# 4. Include Paths
# -----------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/libs/stb_image
)

# -----------------------------------------------------------------------------
# 4.5. Auto-copy shaders and resources on build
# -----------------------------------------------------------------------------

# Find all shader files to track as dependencies
file(GLOB_RECURSE SHADER_FILES
        "${CMAKE_SOURCE_DIR}/shaders/*.vert"
        "${CMAKE_SOURCE_DIR}/shaders/*.frag"
        "${CMAKE_SOURCE_DIR}/shaders/*.geom"
        "${CMAKE_SOURCE_DIR}/shaders/*.glsl"
)

# Find all resource files to track as dependencies
file(GLOB_RECURSE RESOURCE_FILES
        "${CMAKE_SOURCE_DIR}/resources/*"
)

# Create custom target for copying assets
add_custom_target(copy_assets ALL
        COMMENT "Copying shaders and resources to build directory"
        DEPENDS ${SHADER_FILES} ${RESOURCE_FILES}
)

# Copy shaders with directory structure preserved
add_custom_command(
        TARGET copy_assets PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
        COMMENT "Copying shaders directory"
)

# Copy resources with directory structure preserved
add_custom_command(
        TARGET copy_assets PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
        COMMENT "Copying resources directory"
)

# Make sure assets are copied before building the main executable
add_dependencies(${PROJECT_NAME} copy_assets)

# -----------------------------------------------------------------------------
# 5. Link Dependencies
# -----------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glad
        glfw
        imgui          # Added ImGui support
        assimp         # Added Assimp support
        ${CMAKE_DL_LIBS}  # Needed for dynamic linking (e.g., on Linux)
)

# -----------------------------------------------------------------------------
# 6. macOS Framework Linking (Required by GLFW)
# -----------------------------------------------------------------------------

if (APPLE)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework OpenGL"
    )
endif()

# -----------------------------------------------------------------------------
# 7. Test Executable
# -----------------------------------------------------------------------------

# Collect test files
file(GLOB TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

# Only create test executable if test files exist
if(TEST_SOURCES)
    add_executable(
            GLBase_tests
            ${TEST_SOURCES}
            # Include any source files that tests need (excluding main.cpp)
            ${CMAKE_SOURCE_DIR}/libs/stb_image/stb_image.cpp
    )

    target_include_directories(GLBase_tests PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/libs/stb_image
    )

    target_link_libraries(
            GLBase_tests
            PRIVATE
            gtest_main
            glad
            glfw
            imgui          # Added ImGui support for tests
            assimp         # Added Assimp support for tests
            ${CMAKE_DL_LIBS}
    )

    # macOS frameworks for tests too
    if (APPLE)
        target_link_libraries(GLBase_tests
                PRIVATE
                "-framework Cocoa"
                "-framework IOKit"
                "-framework CoreVideo"
                "-framework OpenGL"
        )
    endif()

    # Register tests with CTest
    include(GoogleTest)
    gtest_discover_tests(GLBase_tests)

    message(STATUS "Tests enabled - found test files in tests/ directory")
else()
    message(STATUS "No test files found in tests/ directory - skipping test build")
endif()