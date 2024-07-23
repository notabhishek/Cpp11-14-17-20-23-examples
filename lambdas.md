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

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, decltype([](int x) { return x * x; })> function_map;

    function_map["square"] = [](int x) { return x * x; };
    function_map["cube"] = [](int x) { return x * x * x; };
    function_map["double"] = [](int x) { return x * 2; };

    int value = 5;
    for (const auto& [name, func] : function_map) {
        std::cout << name << " of " << value << ": " << func(value) << "\n";
    }

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

#### Practical Use Cases:

1. **Capturing class members in asynchronous operations:**

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

class AsyncProcessor {
private:
    int data = 0;

public:
    void process_async() {
        auto future = std::async(std::launch::async, [=, this]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            data *= 2;
            std::cout << "Processed data: " << data << std::endl;
        });

        std::cout << "Processing started asynchronously..." << std::endl;
        future.wait();
    }

    void set_data(int value) {
        data = value;
    }

    int get_data() const {
        return data;
    }
};

int main() {
    AsyncProcessor processor;
    processor.set_data(21);
    processor.process_async();
    std::cout << "Final data: " << processor.get_data() << std::endl;

    return 0;
}
```

2. **Event handlers with object lifetime management:**

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <memory>

class Button {
public:
    using ClickHandler = std::function<void()>;

    void set_on_click(ClickHandler handler) {
        on_click = std::move(handler);
    }

    void click() {
        if (on_click) {
            on_click();
        }
    }

private:
    ClickHandler on_click;
};

class UI {
public:
    UI() : count(0) {
        button = std::make_unique<Button>();
        button->set_on_click([=, this]() {
            ++count;
            std::cout << "Button clicked " << count << " times!" << std::endl;
        });
    }

    void simulate_clicks(int n) {
        for (int i = 0; i < n; ++i) {
            button->click();
        }
    }

private:
    std::unique_ptr<Button> button;
    int count;
};

int main() {
    UI ui;
    ui.simulate_clicks(5);

    return 0;
}
```

3. **Memoization with class state:**

```cpp
#include <iostream>
#include <unordered_map>
#include <functional>

class Memoizer {
private:
    std::unordered_map<int, int> cache;

public:
    std::function<int(int)> memoize(std::function<int(int)> func) {
        return [=, this](int arg) mutable {
            if (cache.find(arg) == cache.end()) {
                cache[arg] = func(arg);
            }
            return cache[arg];
        };
    }

    void clear_cache() {
        cache.clear();
    }

    size_t cache_size() const {
        return cache.size();
    }
};

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    Memoizer memo;
    auto memoized_fib = memo.memoize(fibonacci);

    std::cout << "Fibonacci(10): " << memoized_fib(10) << std::endl;
    std::cout << "Fibonacci(20): " << memoized_fib(20) << std::endl;
    std::cout << "Cache size: " << memo.cache_size() << std::endl;

    memo.clear_cache();
    std::cout << "Cache cleared. New size: " << memo.cache_size() << std::endl;

    return 0;
}
```

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
