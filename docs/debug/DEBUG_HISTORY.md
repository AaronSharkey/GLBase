# Common Debugging Mistakes and How to Avoid Them

## 1. File Not Found / Wrong Working Directory

**Symptom:**  
Shader files or other resources not loading, file not found errors.

**Cause:**  
Relative paths are resolved from the working directory, which may be the build directory (e.g., `cmake-build-debug`), not the project root.

**How to Avoid:**
- Use CMake to copy resources (e.g., `file(COPY ...)`) to the build directory.
- Always check your run configuration’s working directory.
- Use absolute paths for critical resources if needed.

---

## 2. OpenGL Context Not Initialized

**Symptom:**  
`EXC_BAD_ACCESS`, segmentation faults, or OpenGL calls failing.

**Cause:**  
Calling OpenGL functions (e.g., creating shaders) before initializing the OpenGL context.

**How to Avoid:**
- Only create OpenGL resources after `glfwInit()`, `glfwCreateWindow()`, and `glfwMakeContextCurrent()`.

---

## 3. Resource Cleanup

**Symptom:**  
Memory/resource leaks, warnings on exit.

**Cause:**  
Not deleting OpenGL resources (shaders, programs, buffers).

**How to Avoid:**
- Implement destructors for resource-managing classes.
- Use RAII patterns in C++.

---

## 4. Error Handling

**Symptom:**  
Silent failures, hard-to-debug issues.

**Cause:**  
Not checking return values or error logs.

**How to Avoid:**
- Always check and log shader compilation/linking status.
- Use exceptions or error codes for file I/O.

---

_Add more sections as you encounter new issues!_