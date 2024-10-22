# C++20 Lambdas: Improvements and Practical Usage

C++20 introduces several improvements to lambdas, making them more powerful and flexible than in previous C++ versions. This document explores these enhancements in depth and provides practical examples of how to use them effectively.

## Key Improvements in C++20 Lambdas

1. Template lambdas
2. Lambdas in unevaluated contexts
3. Default constructible and assignable lambdas
4. Ability to capture `[=, this]`
5. Lambdas in constexpr contexts

Let's dive into each of these improvements with detailed explanations and multiple examples.

### 1. Template Lambdas

C++20 allows you to use `auto` parameters in lambda expressions, effectively creating template lambdas. This feature provides more control over type deduction and enables you to work with the actual types of the lambda parameters.

#### Basic Example:

```cpp
// C++14
auto lambda14 = [](auto x, auto y) { return x + y; };

// C++20
auto lambda20 = []<typename T, typename U>(T x, U y) { return x + y; };
```

#### Practical Use Cases:

1. **Type-based operations:**

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

int main() {
    auto type_checker = []<typename T>(T value) {
        if constexpr (std::is_integral_v<T>) {
            std::cout << "Integer type: " << value << std::endl;
        } else if constexpr (std::is_floating_point_v<T>) {
            std::cout << "Floating-point type: " << value << std::endl;
        } else {
            std::cout << "Other type" << std::endl;
        }
    };

    type_checker(42);       // Integer type: 42
    type_checker(3.14);     // Floating-point type: 3.14
    type_checker("Hello");  // Other type

    return 0;
}
```

2. **Working with containers:**

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <numeric>

int main() {
    auto print_container = []<typename Container>(const Container& c) {
        std::cout << "Container of size " << c.size() << ": ";
        for (const auto& elem : c) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    };

    auto sum_container = []<typename Container>(const Container& c) -> typename Container::value_type {
        return std::accumulate(c.begin(), c.end(), typename Container::value_type{});
    };

    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<double> lst = {1.1, 2.2, 3.3, 4.4, 5.5};

    print_container(vec);  // Container of size 5: 1 2 3 4 5
    print_container(lst);  // Container of size 5: 1.1 2.2 3.3 4.4 5.5

    std::cout << "Sum of vector: " << sum_container(vec) << std::endl;  // Sum of vector: 15
    std::cout << "Sum of list: " << sum_container(lst) << std::endl;    // Sum of list: 16.5

    return 0;
}
```

### 2. Lambdas in Unevaluated Contexts

C++20 allows lambdas to appear in unevaluated contexts, such as the `sizeof` operator, `decltype` specifier, or `noexcept` operator. This feature enables you to work with lambda types without actually invoking the lambda.

#### Basic Example:

```cpp
// C++17: Error
// auto size = sizeof([]() { });

// C++20: OK
auto size = sizeof([]() { });
```

#### Practical Use Cases:

1. **Checking lambda properties:**

```cpp
#include <iostream>
#include <type_traits>

int main() {
    auto lambda = [](int x) { return x * x; };

    std::cout << "Size of lambda: " << sizeof(lambda) << " bytes\n";

    using LambdaType = decltype(lambda);
    std::cout << "Lambda is default constructible: " 
              << std::is_default_constructible_v<LambdaType> << "\n";
    std::cout << "Lambda is copy constructible: " 
              << std::is_copy_constructible_v<LambdaType> << "\n";
    std::cout << "Lambda is move constructible: " 
              << std::is_move_constructible_v<LambdaType> << "\n";

    return 0;
}
```

2. **Using lambdas in type traits:**

```cpp
#include <iostream>
#include <type_traits>

template<typename F, typename... Args>
using invoke_result_t = typename std::invoke_result<F, Args...>::type;

int main() {
    auto plus = [](int a, int b) { return a + b; };
    auto square = [](int x) { return x * x; };

    using PlusResult = invoke_result_t<decltype(plus), int, int>;
    using SquareResult = invoke_result_t<decltype(square), int>;

    std::cout << "Plus result is int: " << std::is_same_v<PlusResult, int> << "\n";
    std::cout << "Square result is int: " << std::is_same_v<SquareResult, int> << "\n";

    return 0;
}
```

3. **Compile-time lambda type checks:**

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
void check_callable() {
    constexpr bool is_callable = std::is_invocable_v<T>;
    constexpr bool returns_int = std::is_same_v<std::invoke_result_t<T>, int>;

    std::cout << "Is callable: " << is_callable << "\n";
    std::cout << "Returns int: " << returns_int << "\n";
}

int main() {
    auto lambda1 = []() { return 42; };
    auto lambda2 = [](int x) { return x * 2; };

    std::cout << "Lambda1:\n";
    check_callable<decltype(lambda1)>();

    std::cout << "\nLambda2:\n";
    check_callable<decltype(lambda2)>();

    return 0;
}
```

### 3. Default Constructible and Assignable Lambdas

In C++20, stateless lambdas (those without captures) are default constructible and assignable. This feature makes lambdas more flexible and easier to use with standard library components that require default constructible types.

#### Basic Example:

```cpp
// C++17: Error
// auto lambda1 = [](int x) { return x * x; };
// decltype(lambda1) lambda2;

// C++20: OK
auto lambda1 = [](int x) { return x * x; };
decltype(lambda1) lambda2;
lambda2 = lambda1;
```

#### Practical Use Cases:

1. **Using lambdas with standard containers:**

https://godbolt.org/z/vMdda3j1W
```cpp
#include <iostream>
#include <map>
#include <string>
#include <functional>

//  Wrapper class for lambda
class LambdaWrapper {
public:
    using FuncType = std::function<int(int)>;

    LambdaWrapper() : func_(defaultFunc) {}

    LambdaWrapper(FuncType func) : func_(func) {}

    int operator()(int x) const {
        return func_(x);
    }

private:
    static int defaultFunc(int x) {
        return x; // Default function
    }

    FuncType func_;
};

int main() {
    // LambdaWrapper as the value type (can be default constructed, constructed with a lambda)
    std::map<std::string, LambdaWrapper> function_map;

    // Initialize map with specific lambdas
    function_map.emplace("square",[](int x) { return x * x; });
    function_map.emplace("cube", [](int x) { return x * x * x; });
    function_map.emplace("double", [](int x) { return x * 2; });

    int value = 5;
    for (const auto& [name, func] : function_map) {
        std::cout << name << " of " << value << ": " << func(value) << "\n";
    }

    // Testing default function for a non-existent key
    std::cout << "undefined operation on " << value << ": " << function_map["undefined"](value) << "\n";

    return 0;
}
```

2. **Lambda factories:**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

auto make_multiplier(int factor) {
    return [factor](int x) { return x * factor; };
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto double_it = make_multiplier(2);
    auto triple_it = make_multiplier(3);

    std::cout << "Original numbers: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    std::transform(numbers.begin(), numbers.end(), numbers.begin(), double_it);
    std::cout << "After doubling: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    std::transform(numbers.begin(), numbers.end(), numbers.begin(), triple_it);
    std::cout << "After tripling: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    return 0;
}
```

### 4. Ability to Capture `[=, this]`

C++20 allows explicitly capturing `this` along with a copy capture default. This feature provides more control over how class members are captured in lambdas defined within member functions.

#### Basic Example:

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

In C++17:
- Capturing `=` would capture all local variables by value.
- Capturing `this` would capture the current object pointer.
- There was no way to capture `*this` (the entire object) by value.

In C++20:
- Capturing `=` captures all local variables by value, including `*this` if used inside a member function.
- Capturing `this` now captures `*this` by reference (equivalent to `*this` capture in C++17).
- A new `*this` capture was introduced to capture the entire object by value.

#### Practical Use Cases:
Here's an example to demonstrate these differences:

```cpp
#include <iostream>

class Example {
private:
    int value;

public:
    Example(int v) : value(v) {}

    void demonstrate() {
        // C++17 style (now deprecated in C++20)
        int x = 5; 
        auto lambda17 = [=]() {  // Warning in C++20
            std::cout << "C++17 lambda: " << value << " x=" << x <<  std::endl;
        };

        // Correct C++20 style
        auto lambda20 = [=, this]() {  // Explicitly capture this
            std::cout << "C++20 lambda: " << value << " x=" << x << std::endl;
        };

        auto lambda20_ref = [this]() {
            std::cout << "C++20 lambda with 'this': " << value << std::endl;
        };

        auto lambda20_copy = [*this]() {
            std::cout << "C++20 lambda with '*this': " << value << std::endl;
        };
		
        x = 10;
        value = 42;

        lambda17();  // Not recommended in C++20
        lambda20();
        lambda20_ref();
        lambda20_copy();
    }
};

int main() {
    Example ex(10);
    ex.demonstrate();
    return 0;
}
```

Let's break down what happens in each case:

1. `lambda17`: In C++17, this captures `this` by value, allowing access to `value` through the implicit `this->value`.

2. `lambda20`: In C++20, this captures `*this` by value, making a copy of the entire object.

3. `lambda20_this`: This captures `*this` by reference, similar to how `this` was captured in C++17.

4. `lambda20_star_this`: This explicitly captures `*this` by value, making a copy of the entire object.

When we run this code, the output will be:

```
C++17 lambda: 42
C++20 lambda: 10
C++20 lambda with 'this': 42
C++20 lambda with '*this': 10
```

The key differences:

- The C++17 lambda sees the updated value (42) because it captured `this` and accesses the original object's data.
- The C++20 lambda with `[=]` captures a copy of the entire object, so it retains the original value (10).
- The C++20 lambda with `[this]` behaves like the C++17 lambda, seeing the updated value (42).
- The C++20 lambda with `[*this]` explicitly captures a copy of the object, retaining the original value (10).

These changes in C++20 provide more clarity and control over how object data is captured in lambdas, especially when dealing with member functions.

### 5. Lambdas in Constexpr Contexts

C++20 allows lambdas to be used in constexpr contexts, enabling compile-time evaluation of lambda expressions. This feature can lead to more efficient code and enables complex compile-time computations.

#### Basic Example:

```cpp
// C++17: Error
// constexpr auto square = [](int x) { return x * x; };

// C++20: OK
constexpr auto square = [](int x) { return x * x; };
```

#### Detailed Explanation:

Constexpr lambdas in C++20 allow for more sophisticated compile-time computations. They can be used in contexts where compile-time evaluation is required, such as template arguments, array sizes, and other constant expressions. This feature enables developers to move more work to compile-time, potentially improving runtime performance.

Key points about constexpr lambdas:
1. They can be used in constant expressions.
2. They can capture variables, but only if those variables are themselves constexpr.
3. They can be used recursively in constexpr contexts.
4. They can be used with STL algorithms in constexpr contexts.

#### Practical Use Cases:

1. **Compile-time computations:**

```cpp
#include <iostream>
#include <array>

constexpr auto factorial = [](int n) {
  auto factorial_impl=[](int n, auto& factorial_ref) {
    if(n <= 1) { return 1; }
    return n * factorial_ref(n-1, factorial_ref);
  };
  return factorial_impl(n,factorial_impl);
};

int main() {
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

    // Compile-time assertion
    static_assert(factorial(5) == 120, "Factorial computation is incorrect");

    return 0;
}
```

## Conclusion

C++20 lambdas offer significant improvements over their predecessors, providing more flexibility and power to C++ developers. These enhancements allow for more expressive and efficient code, particularly in template metaprogramming, compile-time computations, and when working with standard library components.

#### Key takeaways:
1. Template lambdas provide more control over type deduction and enable working with actual parameter types.
2. Lambdas in unevaluated contexts allow for better type traits and compile-time checks.
3. Default constructible and assignable lambdas make it easier to work with standard containers and algorithms.
4. The ability to capture `[=, this]` provides more control over class member captures in lambdas.
5. Constexpr lambdas enable sophisticated compile-time computations and metaprogramming.

By leveraging these new features, developers can write cleaner, more maintainable, and potentially more performant code. The examples provided in this guide demonstrate practical applications of these improvements, showcasing how they can be used to solve real-world problems and enhance C++ codebases.
