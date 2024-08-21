# C++20 Coroutines: An In-Depth Guide

## Introduction to Coroutines
Coroutines are functions that can be suspended and resumed, allowing for more efficient and modular asynchronous programming. The concept was introduced by Melvin Conway in 1958.

### Basic Structure of Coroutines
In C++20, coroutines are defined using the keywords `co_await`, `co_yield`, and `co_return`.

```cpp
// Example of a simple coroutine
CoroIF coro() {
    int i = 0;
    std::cout << ++i << " Hello\n";
    co_await std::suspend_always{};  // Suspend
    std::cout << ++i << " World\n";
    co_await std::suspend_always{};  // Suspend
    std::cout << ++i << " !\n";
}
```

### Key Concepts
- **co_await**: Suspends execution until the awaited operation completes.
- **co_yield**: Produces a value and suspends execution.
- **co_return**: Completes the coroutine.

## Eager Coroutines
Eager coroutines are started immediately when they are called.

### Example of Using Eager Coroutines

```cpp
CoroIF coro() {
    int i = 0;
    std::cout << "  " << ++i << " Hello\n";
    co_await std::suspend_always{};  // SUSPEND
    std::cout << "  " << ++i << " World\n";
    co_await std::suspend_always{};  // SUSPEND
    std::cout << "  " << ++i << " !\n";
}

int main() {
    std::cout << "START\n";
    CoroIF coroTask = coro();
    std::cout << "INIT DONE\n";
    while (coroTask.isResumable()) {
        std::cout << "RESUME\n";
        coroTask.resume();  // RESUME
        std::cout << "BACK\n";
    }
    std::cout << "END\n";
}
```

## Coroutine Interface and Promise Type
A coroutine's behavior and state are controlled by a promise type.

### Coroutine Frame
When a coroutine is started, a frame is created which holds all parameters and a promise to manage its execution.

### Example of Coroutine and Promise API
```cpp
#include <coroutine>

class [[nodiscard]] CoroIF {
public:
    struct promise_type {
        // customization point
    };
    using CoroT = std::coroutine_handle<promise_type>;
    CoroT hdl;

    CoroIF(CoroT h) : hdl{h} {}
    ~CoroIF() { if (hdl) hdl.destroy(); }

    bool isResumable() const { return hdl && !hdl.done(); }
    void resume() { hdl.resume(); }
};
```

## Issues with Coroutines
- **Memory Management**: Managing the lifetime and memory of coroutine frames can be complex.
- **Lack of Coroutine Interface Types**: No standard coroutine interface types in C++20. However, `std::generator<>` is expected in C++23.

## Practical Considerations
- **Avoiding Call-by-Reference**: Be cautious with call-by-reference in coroutines. Always pass by value or use `std::ref()`.
- **Range-Based for Loop**: Avoid using references to temporaries in range-based for loops within coroutines.

### Example of Incorrect Use
```cpp
std::generator<int> coro(const int& num) {
    for(int i = 0; i < num; ++i) {
        co_yield i;
    }
}

// Causes runtime error due to undefined behavior
for (auto val : coro(10)) {
    foo(val);
}
```

### Correct Approach
```cpp
std::vector<int> getTemporaryColl();
auto gen = nextElem(getTemporaryColl());
// Ensure the temporary collection is not destroyed prematurely
```

## Summary
C++20 introduces coroutines as a powerful tool for asynchronous programming, with a focus on efficient and modular code. However, developers must be mindful of memory management and avoid pitfalls like improper use of call-by-reference and temporary objects.

For further details and examples, refer to:
- [C++20 Coroutines](https://wg21.link/p2168)
- [GitHub Repositories](https://github.com/lewissbaker/cppcoro)
```

This detailed presentation covers the fundamental concepts, examples, and practical considerations for using coroutines in C++20.