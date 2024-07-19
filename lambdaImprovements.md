# C++20 Lambdas: Improvements and Practical Usage

C++20 introduces several improvements to lambdas, making them more powerful and flexible than in previous C++ versions. This document explores these enhancements and provides practical examples of how to use them effectively.

## Key Improvements in C++20 Lambdas

1. Template lambdas
2. Lambdas in unevaluated contexts
3. Default constructible and assignable lambdas
4. Ability to capture `[=, this]`
5. Lambdas in constexpr contexts

Let's dive into each of these improvements with examples.

### 1. Template Lambdas

C++20 allows you to use `auto` parameters in lambda expressions, effectively creating template lambdas.

```cpp
// C++14
auto lambda14 = [](auto x, auto y) { return x + y; };

// C++20
auto lambda20 = []<typename T, typename U>(T x, U y) { return x + y; };
```

**Practical use case:**
Template lambdas are particularly useful when you need to access the type information of the arguments.

```cpp
#include <vector>
#include <iostream>

int main() {
    auto print_vector = []<typename T>(const std::vector<T>& vec) {
        std::cout << "Vector of " << typeid(T).name() << ":\n";
        for (const auto& elem : vec) {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    };

    std::vector<int> int_vec = {1, 2, 3, 4, 5};
    std::vector<double> double_vec = {1.1, 2.2, 3.3, 4.4, 5.5};

    print_vector(int_vec);
    print_vector(double_vec);

    return 0;
}
```

### 2. Lambdas in Unevaluated Contexts

C++20 allows lambdas to appear in unevaluated contexts, such as the `sizeof` operator or `decltype` specifier.

```cpp
// C++17: Error
// auto size = sizeof([]() { });

// C++20: OK
auto size = sizeof([]() { });
```

**Practical use case:**
This feature is useful when you need to work with the type of a lambda or its size without invoking it.

```cpp
#include <iostream>
#include <type_traits>

int main() {
    auto lambda = [](int x) { return x * x; };

    std::cout << "Size of lambda: " << sizeof(lambda) << " bytes\n";

    using LambdaType = decltype(lambda);
    std::cout << "Lambda is default constructible: " 
              << std::is_default_constructible_v<LambdaType> << "\n";

    return 0;
}
```

### 3. Default Constructible and Assignable Lambdas

In C++20, stateless lambdas (those without captures) are default constructible and assignable.

```cpp
// C++17: Error
// auto lambda1 = [](int x) { return x * x; };
// decltype(lambda1) lambda2;

// C++20: OK
auto lambda1 = [](int x) { return x * x; };
decltype(lambda1) lambda2;
lambda2 = lambda1;
```

**Practical use case:**
This feature is particularly useful when working with standard library components that require default constructible types.

```cpp
#include <map>
#include <string>
#include <iostream>

int main() {
    std::map<std::string, decltype([](int x) { return x * x; })> function_map;

    function_map["square"] = [](int x) { return x * x; };
    function_map["cube"] = [](int x) { return x * x * x; };

    std::cout << "Square of 5: " << function_map["square"](5) << "\n";
    std::cout << "Cube of 5: " << function_map["cube"](5) << "\n";

    return 0;
}
```

### 4. Ability to Capture `[=, this]`

C++20 allows explicitly capturing `this` along with a copy capture default.

```cpp
struct S {
    int value = 42;

    auto get_lambda() {
        // C++17: Error
        // return [=, this] { return value; };

        // C++20: OK
        return [=, this] { return value; };
    }
};
```

**Practical use case:**
This feature is useful when you want to capture `this` by value in a class member function, ensuring that the lambda doesn't outlive the object it captures.

```cpp
#include <iostream>
#include <functional>

class Counter {
private:
    int count = 0;

public:
    std::function<void()> increment() {
        return [=, this] {
            ++count;
            std::cout << "Count: " << count << "\n";
        };
    }

    int get_count() const { return count; }
};

int main() {
    Counter c;
    auto inc = c.increment();

    inc();  // Count: 1
    inc();  // Count: 2
    inc();  // Count: 3

    std::cout << "Final count: " << c.get_count() << "\n";

    return 0;
}
```

### 5. Lambdas in Constexpr Contexts

C++20 allows lambdas to be used in constexpr contexts, enabling compile-time evaluation of lambda expressions.

```cpp
// C++17: Error
// constexpr auto square = [](int x) { return x * x; };

// C++20: OK
constexpr auto square = [](int x) { return x * x; };
```

**Practical use case:**
Constexpr lambdas are particularly useful for compile-time computations and optimizations.

```cpp
#include <iostream>
#include <array>

int main() {
    constexpr auto factorial = [](int n) {
        if (n <= 1) return 1;
        return n * factorial(n - 1);
    };

    constexpr std::array<int, 5> factorials = {
        factorial(1),
        factorial(2),
        factorial(3),
        factorial(4),
        factorial(5)
    };

    for (size_t i = 0; i < factorials.size(); ++i) {
        std::cout << i + 1 << "! = " << factorials[i] << "\n";
    }

    return 0;
}
```

## Conclusion

C++20 lambdas offer significant improvements over their predecessors, providing more flexibility and power to C++ developers. These enhancements allow for more expressive and efficient code, particularly in template metaprogramming, compile-time computations, and when working with standard library components. By leveraging these new features, developers can write cleaner, more maintainable, and potentially more performant code.
