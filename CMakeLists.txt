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
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

# -----------------------------------------------------------------------------
# 4. Include Paths
# -----------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Copy shaders directory to binary output directory for runtime access
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# -----------------------------------------------------------------------------
# 5. Link Dependencies
# -----------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glad
        glfw          # May be system-installed or later replaced with submodule
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
