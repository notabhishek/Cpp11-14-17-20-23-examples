# C++20 Concepts, Constraints, and Requirements

## Introduction to Concepts

Concepts are a powerful feature introduced in C++20 that allow you to specify constraints on template parameters. They improve code clarity, provide better error messages, and enable more expressive template metaprogramming.

### Basic Syntax and Usage

```cpp
#include <concepts>
#include <iostream>

template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
T add(T a, T b) {
    return a + b;
}

int main() {
    std::cout << add(5, 3) << '\n';      // OK
    std::cout << add(3.14, 2.5) << '\n'; // OK
    // std::cout << add("Hello", "World") << '\n'; // Compile-time error
}
```

## Basic Concept Definition
Concepts can be defined using the `concept` keyword and can incorporate type traits, requires clauses, and other concepts.

### Syntax 1: Using the `concept` keyword

```cpp
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};
```

### Syntax 2: Using `constexpr bool`

```cpp
template<typename T>
constexpr bool Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};
```

## Using Concepts in Function Templates

### Syntax 1: Concept after the template parameter

```cpp
template<typename T>
    requires Addable<T>
T add(T a, T b) {
    return a + b;
}
```

### Syntax 2: Concept in place of `typename`

```cpp
template<Addable T>
T add(T a, T b) {
    return a + b;
}
```

### Syntax 3: Abbreviated function template

```cpp
Addable auto add(Addable auto a, Addable auto b) {
    return a + b;
}
```

## Combining Concepts

### Using logical operators

```cpp
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<typename T>
concept Printable = requires(std::ostream& os, T x) {
    { os << x } -> std::same_as<std::ostream&>;
};

template<typename T>
concept PrintableNumeric = Numeric<T> && Printable<T>;
```

## Defining Complex Requirements

### Using compound requirements

```cpp
template<typename T>
concept Resizable = requires(T container) {
    { container.size() } -> std::convertible_to<std::size_t>;
    { container.resize(std::size_t{}) } -> std::same_as<void>;
};
```

### Using nested requirements

```cpp
template<typename T>
concept Iterable = requires(T x) {
    { begin(x) } -> std::input_or_output_iterator;
    { end(x) } -> std::sentinel_for<decltype(begin(x))>;
    requires std::same_as<decltype(begin(x)), decltype(end(x))>;
};
```

## Constraining Class Templates

### Syntax 1: Using `requires` clause

```cpp
template<typename T>
    requires Addable<T>
class NumericValue {
    T value;
public:
    NumericValue(T v) : value(v) {}
    T get() const { return value; }
};
```

### Syntax 2: Using concept as a template parameter

```cpp
template<Addable T>
class NumericValue {
    T value;
public:
    NumericValue(T v) : value(v) {}
    T get() const { return value; }
};
```

## Constraining Member Functions

```cpp
template<typename T>
class Container {
public:
    template<typename U>
        requires std::convertible_to<U, T>
    void push(U&& value);
};
```

## Using Concepts with Auto

```cpp
void print_numeric(Numeric auto x) {
    std::cout << "Numeric value: " << x << std::endl;
}
```

## Concepts in Variable Declarations

```cpp
Numeric auto x = 5;
Numeric auto y = 3.14;
```

## Example: Implementing a Generic `max` Function

```cpp
template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

Comparable auto max(Comparable auto a, Comparable auto b) {
    return (a < b) ? b : a;
}
```

## Advanced Example: Concept-based Overloading

```cpp
#include <concepts>
#include <iostream>
#include <vector>
#include <list>

template<typename T>
concept Contiguous = requires(T t) {
    { t.data() } -> std::convertible_to<const typename T::value_type*>;
};

template<typename T>
concept Sequence = std::ranges::forward_range<T>;

void process(Contiguous auto& cont) {
    std::cout << "Processing contiguous container\n";
    // Efficient processing using pointer arithmetic
}

void process(Sequence auto& seq) {
    std::cout << "Processing generic sequence\n";
    // Less efficient, but more general processing
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {1, 2, 3, 4, 5};

    process(vec);  // Calls contiguous version
    process(lst);  // Calls sequence version
}
```

This example demonstrates how concepts can be used for overload resolution, allowing for more efficient implementations when stronger guarantees are available.

Another example: https://bbgb.dev.bloomberg.com/z/26fdb4

## Better error messages

https://bbgb.dev.bloomberg.com/z/fe7d72

```cpp
#include <compare>
#include <vector>
#include <string>
#include <set>

// template<std::ranges::range CollT, typename T> 
// void add(CollT& coll, const T& val) 
// requires std::convertible_to<T, std::ranges::range_value_t<CollT>> {
void add(auto& coll, const auto& val) {
    if constexpr(requires {coll.push_back(val); }) {
        coll.push_back(val);
    }
    else {
        coll.insert(val);
    }
}

int main() {
    std::vector<int> a;
    std::set<std::string> b;
    
    add(a, 43);
    add(b, 43);
    return 0;
}
```

## Performance Considerations

Concepts are evaluated at compile-time and do not incur runtime overhead. However, complex concepts may increase compilation time.

```cpp
#include <concepts>
#include <iostream>
#include <chrono>

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template<Addable T>
T add(T a, T b) {
    return a + b;
}

template<typename T>
T add_non_concept(T a, T b) {
    return a + b;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        volatile auto result = add(5, 3);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time with concept: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
              << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        volatile auto result = add_non_concept(5, 3);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time without concept: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
              << " microseconds\n";
}
```

## Best Practices and Pitfalls

1. Use concepts to document assumptions about template parameters.
2. Prefer simple, composable concepts over complex, monolithic ones.
3. Be aware that concepts only check syntactic requirements, not semantic ones.

## Further reading 
- https://en.cppreference.com/w/cpp/language/constraints
- https://josuttis.com/download/cpponsea24/C++20_cpponsea_240630.pdf
- https://www.sandordargo.com/blog/2021/10/13/cpp-concepts-core-guidelines
