# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and Development Commands

**Build the project:**
```bash
cmake -B cmake-build-debug
cmake --build cmake-build-debug
```

**Run the application:**
```bash
./cmake-build-debug/GLBase
```

**Dependencies:**
- Requires GLFW 3.3+ (install via Homebrew on macOS: `brew install glfw`)
- GLAD is included as a static library in `libs/glad/`

## Architecture Overview

GLBase is a C++20 OpenGL boilerplate project with a clean, object-oriented architecture:

**Core Classes:**
- `Window` (include/window.h): GLFW window management, OpenGL context setup, input handling
- `Shader` (include/shader.h): Shader loading, compilation, and program linking from file paths
- `Mesh` (include/mesh.h): VAO-backed mesh rendering with VBO/EBO support and optional wireframe mode

**Directory Structure:**
- `include/`: Header-only implementations of core classes
- `src/`: Application entry point (main.cpp)
- `shaders/`: GLSL shader source files (copied to build directory at compile time)
- `libs/glad/`: GLAD OpenGL function loader library
- `docs/`: Project documentation including comprehensive C++ glossary

**Key Design Patterns:**
- Header-only implementations for small classes (improves compilation for this scale)
- RAII resource management for OpenGL objects (automatic cleanup in destructors)
- Const-correctness throughout the API
- Member initializer lists for efficient construction

**OpenGL Setup:**
- OpenGL 3.3 Core Profile
- Uses GLAD for function loading
- Shader files loaded from filesystem at runtime
- Viewport automatically resizes with window

**Code Style:**
- C++20 with modern features (`[[nodiscard]]`, `constexpr`, etc.)
- camelCase for methods/variables, PascalCase for classes
- Comprehensive Doxygen documentation
- Explicit constructors to prevent implicit conversions
- Static casts for type safety in OpenGL calls

The project follows the conventions detailed in `docs/GLOSSARY.md` which contains C++ best practices and OpenGL-specific guidance.