# OpenGL Example System - Design Discussion

## Project Context

Building an OpenGL base project that will serve as a foundation for future graphics projects. Current main.cpp is clean
with basic `Window`, `Shader`, and `Mesh` classes.

**Goal**: Create a system to add modular "snippets" of rendering logic instead of writing everything directly in the
render loop.

**Vision**: MVP will be an IMGUI-based graphics demo viewer where users can select different examples to showcase
learned OpenGL techniques.

## Problem Statement

Need to abstract render logic (like time-based uniform animations, input-triggered color changes, vertex oscillations)
into reusable components that can be easily added to projects.

## Design Approaches Considered

### 1. Dependency Injection / Catalog Pattern

- **Concept**: Central "catalog" or "context" object containing all services (shader, mesh, time, input)
- **Pros**: Flexible, can handle unknown future dependencies
- **Cons**: Added complexity, overkill for current scope
- **Example**: `context.get<Shader>()`, `context.get<TimeService>()`

### 2. Direct Parameter Passing ⭐

- **Concept**: Pass exactly what each example needs as parameters
- **Pros**: Simple, explicit, easy to debug, no magic
- **Cons**: Could get verbose with many parameters
- **Example**: `example.render(shader, deltaTime)`

### 3. Observer/Event Pattern

- **Concept**: Examples subscribe to events (time updates, key presses)
- **Pros**: Decoupled, reactive
- **Cons**: Requires event system infrastructure, potential ordering issues
- **Example**: `example.subscribeToTimeUpdates()`

### 4. Functional Approach

- **Concept**: Pass lambdas/functions instead of objects
- **Pros**: Lightweight, functional style
- **Cons**: Harder to maintain state, less object-oriented

### 5. Builder/Fluent Interface

- **Concept**: Chain what each example needs
- **Pros**: Readable, self-documenting
- **Cons**: Complex to implement, potentially verbose

## Selected Approach: Direct Parameter Passing

**Rationale**:

- Aligns with current clean, explicit architecture
- Simple to implement and understand
- Can evolve as needs grow
- Avoiding premature complexity for MVP scope

## Final Architecture

### Base Class

```cpp
class Example {
public:
    virtual void render(float deltaTime) = 0;
    virtual const char* getName() const = 0;  // For IMGUI dropdown
    virtual void reset() = 0;                 // When switching examples
};
```

### Implementation Pattern

Each example owns its resources completely:

```cpp
class PhongLightingDemo : public Example {
private:
    Shader phongShader{"shaders/lighting/phong.vert", "shaders/lighting/phong.frag"};
    Mesh sphereMesh{/* sphere data */};
    
public:
    void render(float deltaTime) override {
        phongShader.use();
        // rendering logic here
        sphereMesh.render();
    }
    const char* getName() const override { return "Phong Lighting"; }
    void reset() override { /* reset state if needed */ }
};
```

### Project Structure

```
project/
├── src/
│   ├── examples/
│   │   ├── TimeDynamicUniformAnimation.h
│   │   ├── RandomColorOnInput.h
│   │   └── VertexOscillation.h
│   ├── shader.h
│   ├── mesh.h
│   ├── window.h
│   └── main.cpp
```

### Main Loop

```cpp
int main() {
    const Window basicWindow;
    
    // IMGUI setup and example selection logic
    
    while (!basicWindow.getShouldClose()) {
        basicWindow.processInput();
        Window::clear();
        
        if (currentExample) {
            currentExample->render(glfwGetTime());
        }
        
        // IMGUI rendering
        basicWindow.swapBuffers();
        Window::pollEvents();
    }
    
    glfwTerminate();
    return 0;
}
```

## Key Design Decisions

1. **Resource Ownership**: Each example owns its shaders and meshes completely
2. **Shared Resources**: Only `Window` is shared infrastructure
3. **Shader Reusability**: Through file system (multiple examples can reference same shader files)
4. **State Management**: Examples are stateful within themselves, no chaining for MVP
5. **Naming**: `Example` chosen over `RenderActivity`, `Demo`, `Sample` to clearly indicate learning/demonstration
   purpose

## Future Considerations

- **Shader Class Evolution**: Current `Shader` class couples vertex + fragment. May need to break into `ShaderProgram`
  and `ShaderStage` for geometry/tessellation/compute shaders
- **Activity Chaining**: Pipeline approach could be explored later for more complex effects
- **Parameter Evolution**: Interface can be extended as examples need more context (input, mouse, etc.)

## Lessons Learned

- Start simple, evolve as needed
- Avoid premature optimization for unknown future requirements
- Explicit is better than magical
- Design for the actual use case (learning showcase) rather than imagined complexity