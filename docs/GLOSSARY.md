# Glossary

## `#pragma once` vs. Include Guards

- `#pragma once` is simpler and widely supported, but not standard.
- Include guards are standard and portable.
- Use one, not both. Modern C++ prefers `#pragma once`.

## Catching Exceptions

- Catch exceptions by `const` reference to avoid slicing and unnecessary copies.
- Example:
  `catch (const std::ifstream::failure& e) { ... }`

## Method Naming Conventions

- Use `camelCase` for methods and variables.
- Use `PascalCase` for class names.
- Example:
  `checkCompileStatus` (method), `Shader` (class)

## If Statement with Initializer (C++17+)

- Scopes variables to the `if` statement.
- Example:
  `if (int status = check(); status > 0) { ... }`

## Const Correctness

- Mark variables, parameters, and member functions as `const` if they should not modify data.
- Improves code safety and clarity; allows compiler to enforce immutability.
- Example:
  `void use() const;`

## Doxygen-style Comments

- Use `///` or `/** ... */` to document classes, methods, and functions.
- Enables automatic API documentation generation.
- Focus comments on intent, purpose, and non-obvious logic.

## RAII (Resource Acquisition Is Initialization)

- Tie resource allocation and deallocation to object lifetime.
- Ensures proper cleanup (e.g., releasing OpenGL resources in a destructor).

## In-class Member Initializer

- Initialize class member variables directly in their declaration.
- Improves clarity and reduces errors.
- Example:
  `int value = 0;`

## Unified Diff

- A text format showing changes between two versions of a file.
- Used in code reviews to highlight additions and deletions.

## Exception Safety

- Write code that behaves correctly in the presence of exceptions.
- Use try-catch blocks and RAII to manage resources safely.

## Grouping Declarations

- Group related variable declarations for clarity and compactness.
- Example:
  `std::string vertexCode, fragmentCode;`

## Commenting Best Practices

- Avoid restating obvious code.
- Use comments to explain why, not what, especially for non-obvious logic.
- Prefer concise, purposeful comments over verbose or redundant ones.

## Inline Headers vs. Separate Implementation Files

- Use inline functions in headers for small, performance-critical, or template code.
- Suitable for simple utility classes, small constructors/destructors, and short methods.
- Keeps `/src` clean by reducing the number of small `.cpp` files.
- For larger or more complex functions, or when hiding implementation details, prefer separate `.cpp` files.
- Moving implementations to `.cpp` files can improve compile times and maintainability as projects grow.

**Example:**

- Inline: Small `Shader` class methods in `shader.h` for a small project.
- Separate: Move to `shader.cpp` if logic grows or is reused widely.

## explicit Keyword

- The `explicit` keyword is used with constructors to prevent implicit conversions and copy-initialization.
- When a constructor is marked `explicit`, code like `Mesh m = vertices;` will not compile; only direct initialization (
  `Mesh m(vertices);`) is allowed.
- This improves type safety and prevents unintended or confusing conversions.

**Example:**

```cpp
explicit Mesh(GLfloat* vertices); // Only direct initialization allowed
```

## Pointer Constness (`const GLfloat* vertices` vs. `const GLfloat* const vertices`)

- `const GLfloat* vertices`: The data pointed to by `vertices` is constant (cannot be modified), but the pointer itself
  can be changed to point elsewhere.
- `const GLfloat* const vertices`: Both the data pointed to and the pointer itself are constant; you cannot modify the
  data nor change what `vertices` points to.

**Example:**

```cpp
void foo(const GLfloat* vertices);        // Can change pointer, not data
void bar(const GLfloat* const vertices);  // Cannot change pointer or data
```

## glVertexAttribPointer and `static_cast<void *>(nullptr)`

- `glVertexAttribPointer` defines an array of generic vertex attribute data for OpenGL.
- The last parameter specifies the offset (in bytes) of the first component of the first attribute in the buffer.
- `static_cast<void *>(nullptr)` is the C++-preferred, type-safe way to specify a null pointer (no offset) for this
  parameter.
- Using `static_cast` is safer and more explicit than a C-style cast like `(void*)nullptr`.

**Example:**

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
```

## Member Initializer List

- A C++ constructor feature that allows direct initialization of class members before the constructor body runs.
- Uses parentheses syntax: `MemberName(value)`.
- More efficient than assignment in the constructor body, especially for `const`, reference, or complex types.
- Required for initializing `const` members and references.

**Example:**

```cpp
class Example {
public:
    Example(int x) : value(x) {}
private:
    const int value;
};
```

## Wireframe Rendering in OpenGL

- Wireframe mode renders only the edges of polygons, useful for debugging or stylistic effects.
- Use `glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)` to enable, and `GL_FILL` to restore normal rendering.
- Change polygon mode only during rendering, not in constructors, to avoid affecting unrelated objects.

## Clang-Tidy: Narrowing Conversion Warning

- Occurs when converting from an unsigned type (like `size_t`) to a signed type (like `GLsizeiptr`).
- Use `static_cast<GLsizeiptr>(...)` to make the conversion explicit and silence the warning.

## OpenGL Buffer Size Idiom

- The standard idiom for buffer size is `count * sizeof(type)`, not `sizeof(type) * count`.
- Improves readability and matches common C/C++ style.

## Indentation for Resource Grouping

- Indent code blocks to visually group OpenGL resource operations (e.g., VAO, VBO, EBO).
- Enhances clarity but does not affect program behavior.

## Most Vexing Parse

- In C++, a declaration like `Window basicWindow();` is interpreted as a function declaration, not as an object
  definition.
- This is known as the "most vexing parse."
- To define an object, omit the parentheses: `Window basicWindow;`
- If the constructor takes arguments, use: `Window basicWindow(arg1, arg2);`
- Avoid empty parentheses when declaring objects to prevent this ambiguity.

## [[nodiscard]]

- A C++ attribute that instructs the compiler to warn if the return value of a function is ignored.
- Helps prevent bugs by ensuring important results (such as error codes or status flags) are not accidentally discarded.
- Commonly used for functions where ignoring the result could lead to incorrect program behavior.

**Example:**

```cpp
[[nodiscard]] bool getShouldClose() const;
```

## Virtual Destructors

- Virtual destructors ensure proper cleanup when deleting objects through base class pointers.
- When a base class has virtual functions, its destructor should also be virtual to enable correct polymorphic
  destruction.
- `virtual ~ClassName() = default;` tells the compiler to generate the standard destructor behavior while maintaining
  virtual behavior.
- Without virtual destructors, only the base class destructor is called, potentially causing resource leaks in derived
  classes.

**Example:**

```cpp
class Base {
public:
    virtual ~Base() = default;  // Ensures proper cleanup chain
};

class Derived : public Base {
    ~Derived() override { /* cleanup derived resources */ }
};

Base* obj = new Derived();
delete obj;  // Calls ~Derived() then ~Base() correctly
```

## Pure Virtual Functions

- Pure virtual functions are declared with `= 0` and have no implementation in the base class.
- Classes containing pure virtual functions become abstract and cannot be instantiated directly.
- Derived classes must provide implementations for all pure virtual functions to become concrete (instantiable).
- Used to define interfaces and enforce implementation requirements in inheritance hierarchies.

**Example:**

```cpp
class Shape {
public:
    virtual void draw() = 0;        // Pure virtual - must be implemented
    virtual double area() = 0;      // Pure virtual - must be implemented
    virtual ~Shape() = default;     // Virtual destructor
};

// Shape s;  // ERROR: Cannot instantiate abstract class

class Circle : public Shape {
public:
    void draw() override { /* implementation required */ }
    double area() override { /* implementation required */ }
};
```

## References vs Pointers

- References (`Type&`) are aliases to existing objects and cannot be null or reassigned.
- Pointers (`Type*`) store memory addresses and can be null, reassigned, or point to different objects.
- References provide cleaner syntax and are safer for function parameters when you don't need null values.
- Use `const Type&` for read-only access to avoid copying large objects.

**Example:**

```cpp
void function(const Config& config);     // Reference - no null, no copy
void function(const Config* config);     // Pointer - can be null, need & when calling
```

## Brace Initialization (`{}`) vs Direct Initialization (`()`)

- Brace initialization `{}` prevents narrowing conversions and avoids the "most vexing parse."
- Direct initialization `()` allows narrowing conversions but can be ambiguous in declarations.
- Brace initialization provides uniform syntax for all types and is preferred in modern C++.
- Use brace initialization as default unless you specifically need direct initialization behavior.

**Example:**

```cpp
int x{3.14};        // ERROR - prevents narrowing
int y(3.14);        // OK but truncates to 3
Widget w{};         // Clear object initialization
Widget v();         // Function declaration (most vexing parse)
```

## emplace_back vs push_back

- `emplace_back()` constructs objects directly in the container's memory using forwarded arguments.
- `push_back()` constructs a temporary object and then copies/moves it into the container.
- `emplace_back()` is more efficient for complex types as it avoids unnecessary copy/move operations.
- For simple types, the performance difference is minimal.

**Example:**

```cpp
std::vector<std::pair<int, string>> vec;
vec.push_back({42, "hello"});           // Creates temporary pair, then moves
vec.emplace_back(42, "hello");          // Constructs pair directly in vector
```

## Texture Units in OpenGL

- OpenGL provides multiple texture units (GL_TEXTURE0, GL_TEXTURE1, etc.) for binding multiple textures simultaneously.
- `glActiveTexture(GL_TEXTURE0 + unit)` selects which texture unit subsequent operations affect.
- Shader uniforms of type `sampler2D` hold integer values indicating which texture unit to sample from.
- The connection is: bind texture to unit → set uniform to unit number → shader samples from that unit.

**Example:**

```cpp
glActiveTexture(GL_TEXTURE0);               // Select unit 0
glBindTexture(GL_TEXTURE_2D, textureID);    // Bind texture to unit 0
glUniform1i(uniformLocation, 0);            // Tell shader to use unit 0
```

## Builder Pattern

- A creational design pattern that constructs complex objects step by step.
- Separates object construction from its representation, allowing the same construction process to create different
  representations.
- Useful when objects require many optional parameters or complex initialization.
- Often implemented with method chaining for fluent interfaces.

**Example:**

```cpp
Mesh mesh{vertices, indices, count};
mesh.addPositionAttributes();      // Configure step by step
mesh.addColorAttributes();         // Rather than complex constructor
```