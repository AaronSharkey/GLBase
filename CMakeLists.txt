cmake_minimum_required(VERSION 3.31)
project(GLBase)

# Enable compile_commands.json for IntelliSense (CLion, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# 1. Build GLAD as a static library
# -----------------------------------------------------------------------------
add_library(glad STATIC
        ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c
)
target_include_directories(glad PUBLIC
        ${CMAKE_SOURCE_DIR}/libs/glad/include
)

# -----------------------------------------------------------------------------
# 2. Find GLFW (system install or fallback to submodule)
# -----------------------------------------------------------------------------
# Tell CMake where to find Homebrew packages (macOS)
if(APPLE)
    set(CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

# Try to find installed GLFW
find_package(glfw3 3.3 QUIET)

# If not found, use submodule - TODO
if (NOT glfw3_FOUND)
    message(STATUS "GLFW not found globally. Falling back to submodule.")
    #add_subdirectory(libs/glfw)
endif()

# -----------------------------------------------------------------------------
# 3. Collect source files
# -----------------------------------------------------------------------------
file(GLOB APP_SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

# -----------------------------------------------------------------------------
# 4. Include directories (only what's needed)
# -----------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# -----------------------------------------------------------------------------
# 5. Link libraries
# -----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glad
        glfw
        ${CMAKE_DL_LIBS} # Only meaningful on Linux, ignored on macOS
)

# -----------------------------------------------------------------------------
# 6. Link macOS frameworks (required by GLFW/OpenGL)
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
