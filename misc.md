# C++20 Metaprogramming

## 1. Introduction to Metaprogramming
Metaprogramming in C++20 enables powerful compile-time computations and code generation, offering significant performance improvements and advanced programming techniques.

### What is Metaprogramming?
Metaprogramming is writing programs that write or manipulate other programs (or themselves) as their data, or that do part of the computation at compile time.

### Importance in C++
Metaprogramming is crucial in C++ for creating highly efficient and reusable code, especially in template programming and compile-time optimizations.

---

## 2. Compile-Time Functions

### `constexpr` Functions
`constexpr` functions can be evaluated at compile time, offering performance benefits by reducing runtime computation.

```cpp
constexpr int constexprFunc(int x) {
    return x * x;
}

int main() {
    constexpr int result = constexprFunc(7); // Evaluated at compile time
    std::cout << result << std::endl;
}
```

### Restrictions
- Only a subset of C++ is allowed inside `constexpr` functions.
- Cannot contain static variables, except for `static constexpr`.

### `consteval` Functions
`consteval` functions are guaranteed to be evaluated at compile time.

```cpp
consteval int constevalFunc(int x) {
    return x * x;
}

int main() {
    constexpr int result = constevalFunc(7); // Must be evaluated at compile time
    std::cout << result << std::endl;
}
```

### Differences from `constexpr`
- `consteval` ensures compile-time evaluation, whereas `constexpr` can fall back to runtime evaluation if needed.

---

## 3. Conditional Compile-Time Functions

### Using `if constexpr`
`if constexpr` allows conditional compilation based on constant expressions.

```cpp
constexpr int compute(int x) {
    if constexpr (x > 10) {
        return x * 2;
    } else {
        return x + 2;
    }
}

int main() {
    constexpr int result = compute(15); // Uses the branch x * 2
    std::cout << result << std::endl;
}
```

### Example: Compile-Time Error Trigger
Creating a compile-time error deliberately for constraint enforcement.

```cpp
consteval void triggerCompileError() {
    static_assert(false, "Triggered compile-time error!");
}

consteval int checkedFunc(int x) {
    if (x < 0) {
        triggerCompileError();
    }
    return x * x;
}
```

### Benefits
- Ensures errors are caught early during compilation.
- Enables complex compile-time logic without runtime overhead.

---

## 4. `std::is_constant_evaluated()`

### Purpose
Determines if a function is being evaluated in a constant expression context.

```cpp
#include <type_traits>

constexpr int length(const char* str) {
    if (std::is_constant_evaluated()) {
        int len = 0;
        while (str[len] != '\0') {
            ++len;
        }
        return len;
    } else {
        return std::strlen(str);
    }
}

int main() {
    constexpr int len1 = length("hello"); // Compile-time
    int len2 = length("world");           // Runtime
    std::cout << len1 << ", " << len2 << std::endl;
}
```

### Use Cases
- Writing functions that adapt to being used in both compile-time and runtime contexts.

---

## 5. Examples and Advanced Usage

### Prime Number Calculation with `consteval`
Using `consteval` and NTTP for compile-time prime number generation.

```cpp
consteval bool isPrime(int value) {
    for (int i = 2; i <= value / 2; ++i) {
        if (value % i == 0) {
            return false;
        }
    }
    return value > 1;
}

consteval auto generatePrimes(int count) {
    std::array<int, count> primes{};
    int found = 0;
    for (int i = 2; found < count; ++i) {
        if (isPrime(i)) {
            primes[found++] = i;
        }
    }
    return primes;
}

int main() {
    constexpr auto primes = generatePrimes(10);
    for (const auto& prime : primes) {
        std::cout << prime << " ";
    }
}
```

### Benefits
- Efficient compile-time computation.
- Generates optimized code by eliminating runtime calculations.

---

## Conclusion
C++20's metaprogramming capabilities, including `constexpr`, `consteval`, and `std::is_constant_evaluated`, provide powerful tools for compile-time computation. These features enable developers to write more efficient, error-free, and maintainable code by leveraging compile-time logic and optimizations.
