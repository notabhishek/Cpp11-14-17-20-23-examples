# C++20 Ranges and Views Overview 

C++20 introduced the Ranges library, a major addition to the C++ standard library that provides powerful tools for working with sequences of elements. This document provides a comprehensive overview of the key features and concepts introduced with ranges and views in C++20.

## 1. Ranges

A range is anything that has a beginning and an end. In C++20, this concept is formalized, allowing for more generic and expressive code when working with sequences of elements.

### 1.1 Range Concepts

C++20 introduces several range concepts:

- `std::ranges::range`: The most basic range concept
- `std::ranges::input_range`: Can be read from
- `std::ranges::forward_range`: Can be traversed multiple times
- `std::ranges::bidirectional_range`: Can be traversed in both directions
- `std::ranges::random_access_range`: Elements can be accessed in constant time
- `std::ranges::contiguous_range`: Elements are stored contiguously in memory
- <img width="635" alt="Screenshot 2024-07-23 at 14 25 40" src="https://github.com/user-attachments/assets/67dc7a10-d417-4909-a0e3-210f228a313b">

```cpp
#include <ranges>
#include <vector>
#include <list>

std::vector<int> vec{1, 2, 3, 4, 5};  // random_access_range and contiguous_range
std::list<int> lst{1, 2, 3, 4, 5};    // bidirectional_range
```

### 1.2 Range-based Algorithms

C++20 introduces range-based versions of all standard algorithms, which can work directly with ranges:

```cpp
#include <algorithm>
#include <ranges>
#include <vector>

std::vector<int> vec{5, 3, 1, 4, 2};
std::ranges::sort(vec);  // No need to pass begin() and end()
std::ranges::reverse(vec);
auto it = std::ranges::find(vec, 3);
```

## 2. Views

Views are lightweight objects that provide a view into a sequence of elements. They are lazy and do not own the elements they refer to.

### 2.1 View Adaptors

C++20 introduces several view adaptors:

#### `views::all`
Creates a view from a range:
```cpp
std::vector<int> vec{1, 2, 3, 4, 5};
auto all_view = std::views::all(vec);
```

#### `views::filter`
Selects elements that satisfy a predicate:
```cpp
auto even = vec | std::views::filter([](int n) { return n % 2 == 0; });
// even: {2, 4}
```

#### `views::transform`
Applies a transformation to each element:
```cpp
auto squared = vec | std::views::transform([](int n) { return n * n; });
// squared: {1, 4, 9, 16, 25}
```

#### `views::take`
Takes the first n elements:
```cpp
auto first_three = vec | std::views::take(3);
// first_three: {1, 2, 3}
```

#### `views::drop`
Drops the first n elements:
```cpp
auto last_two = vec | std::views::drop(3);
// last_two: {4, 5}
```

#### `views::reverse`
Reverses the order of elements:
```cpp
auto reversed = vec | std::views::reverse;
// reversed: {5, 4, 3, 2, 1}
```

#### `views::join`
Flattens a range of ranges:
```cpp
std::vector<std::vector<int>> nested{{1, 2}, {3, 4}, {5, 6}};
auto joined = nested | std::views::join;
// joined: {1, 2, 3, 4, 5, 6}
```

#### Standard range adaptors in cpp20 
https://learn.microsoft.com/en-us/cpp/standard-library/range-adaptors?view=msvc-170

### 2.2 View Composition

Views can be easily composed using the pipe operator (`|`):

```cpp
auto result = vec 
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; })
    | std::views::take(2);
// result: {4, 16}
```
#### C++17 vs C++20 example
Lets say we wanted to read a multiline string, and do the following 
- Trim spaces
- Remove empty lines
- Get word counts

Example c++17 implementation: https://godbolt.org/z/vhcssbsr7
Example c++20 implementation: https://godbolt.org/z/GTWh5MYcz

## 3. Lazy Evaluation

Views are lazily evaluated, meaning that operations are only performed when the elements are actually accessed:

```cpp
std::vector<int> vec{1, 2, 3, 4, 5};
auto view = vec 
    | std::views::filter([](int n) { std::cout << "Filtering " << n << "\n"; return n % 2 == 0; })
    | std::views::transform([](int n) { std::cout << "Transforming " << n << "\n"; return n * n; });

// No output yet, as no elements have been accessed

std::cout << "First element: " << *view.begin() << "\n";
// Output:
// Filtering 1
// Filtering 2
// Transforming 2
// First element: 4
```

## 4. Infinite Ranges

C++20 ranges support working with infinite sequences:

```cpp
auto integers = std::views::iota(1);  // Infinite sequence of integers
auto first_10 = integers | std::views::take(10);
// first_10: {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
```

## 5. Sentinels

C++20 introduces the concept of sentinels, which are a generalization of the idea of the "end" of a range. This allows for more efficient range operations in some cases:

```cpp
const char* find_null(const char* p) {
    while (*p) ++p;
    return p;
}

const char* str = "Hello, world!";
auto char_range = std::ranges::subrange(str, find_null);
```

## 6. Projections

Many range-based algorithms in C++20 support projections, which allow you to specify how the elements should be viewed by the algorithm:

```cpp
struct Person {
    std::string name;
    int age;
};

std::vector<Person> people = {{"Alice", 25}, {"Bob", 30}, {"Charlie", 20}};
std::ranges::sort(people, {}, &Person::age);  // Sort by age
```

## 7. Performance Considerations

While ranges can often lead to more efficient code due to lazy evaluation, it's important to be aware of potential performance implications.

```cpp
#include <ranges>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <numeric>

int main() {
    std::vector<int> vec(1'000'000);
    std::iota(vec.begin(), vec.end(), 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Using ranges
    auto view = vec | std::views::filter([](int n) { return n % 2 == 0; })
             | std::views::transform([](int n) { return n * n; });

    auto sum_ranges = std::accumulate(
        view.begin(),
        view.end(),
        0
    );
    
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Ranges time: " 
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() 
              << "ns\n";
    
    start = std::chrono::high_resolution_clock::now();
    
    // Traditional approach
    int sum_traditional = 0;
    for (int n : vec) {
        if (n % 2 == 0) {
            sum_traditional += n * n;
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Traditional time: " 
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() 
              << "ns\n";
    
    std::cout << "Sum (ranges): " << sum_ranges << '\n';
    std::cout << "Sum (traditional): " << sum_traditional << '\n';
}
```

### Const-Correctness Issues with Views

Some views in C++20 cannot be passed as const references due to their internal implementation. This can lead to unexpected behavior and compilation errors.

#### Example of the Problem

```cpp
#include <ranges>
#include <vector>

void process(const auto& view) {
    for (const auto& item : view) {
        // Process item
    }
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto filtered = numbers | std::views::filter([](int n) { return n % 2 == 0; });
    
    process(filtered); // This will not compile
}
```

The above code will not compile because `std::views::filter` returns a view that cannot be used as a const reference.

#### Explanation

The issue arises because some view adaptors, like `filter` and `transform`, store their state in mutable members. This is necessary for lazy evaluation and to maintain the non-owning, lightweight nature of views.

When you call `begin()` on such a view, it may need to update its internal state (e.g., to find the first element that satisfies the filter). If the view were const, this state update would not be possible.

#### Workarounds

1. Pass views by value instead of const reference:

```cpp
void process(auto view) {
    for (const auto& item : view) {
        // Process item
    }
}
```

2. Use `std::ranges::ref_view` to wrap the original range:

```cpp
void process(const auto& view) {
    for (const auto& item : view) {
        // Process item
    }
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto filtered = std::ranges::ref_view(numbers) 
        | std::views::filter([](int n) { return n % 2 == 0; });
    
    process(filtered); // This will compile
}
```

### Design Changes: Begin Caching

To address performance concerns and maintain the lazy evaluation property of views, C++20 introduced the concept of "begin caching" for certain view adaptors.

#### What is Begin Caching?

Begin caching is a technique where a view stores (caches) the result of the first call to `begin()`. This cached iterator is then returned on subsequent calls to `begin()`, avoiding redundant computation.

#### Why is it Necessary?

Consider a chain of view adaptors:

```cpp
auto view = numbers 
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; });
```

Without begin caching, each call to `begin()` would have to traverse the entire chain of adaptors, potentially performing expensive operations multiple times.

#### How it Works

1. The first time `begin()` is called on a view with begin caching:
   - The view performs necessary computations to find the first element.
   - It stores the resulting iterator.

2. On subsequent calls to `begin()`:
   - The view returns the stored iterator.

#### Example of Begin Caching

```cpp
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto view = numbers 
        | std::views::filter([](int n) { 
            std::cout << "Filtering " << n << std::endl;
            return n % 2 == 0; 
          })
        | std::views::transform([](int n) { 
            std::cout << "Transforming " << n << std::endl;
            return n * n; 
          });
    
    // First call to begin() - will print filtering and transforming messages
    auto it = std::ranges::begin(view);
    std::cout << *it << std::endl;
    
    // Second call to begin() - will not print messages, uses cached iterator
    it = std::ranges::begin(view);
    std::cout << *it << std::endl;
}
```

In this example, you'll see that the filtering and transforming messages are only printed once, even though we call `begin()` twice.

#### Implications of Begin Caching

1. **Performance Improvement**: Avoid redundant computations on repeated traversals.
2. **Consistency**: Ensures that multiple traversals of the same view yield consistent results.
3. **Complexity**: Adds some implementation complexity to view adaptors.

### Design Considerations

1. **Trade-offs**: Begin caching improves performance but increases memory usage slightly.
2. **Invalidation**: Modifying the underlying range can invalidate the cached begin iterator.
3. **Non-const Views**: The need for begin caching is one reason why some views can't be const.

## 8. Best Practices and Pitfalls

1. Use ranges to simplify complex algorithms and improve readability.
2. Be cautious of creating infinite ranges without proper termination conditions.
3. Remember that views are lazy; they don't perform any work until iterated.
4. Use `std::views::common` when interfacing with legacy code expecting begin/end pairs.
5. Be aware that some views may change the category of a range (e.g., from random-access to bidirectional).
6. Const correctness with views (see previous section)
7. <img width="627" alt="Screenshot 2024-07-23 at 14 32 14" src="https://github.com/user-attachments/assets/dbecf40e-f501-40c4-af4d-2a116f9bbb9f">
8. <img width="616" alt="Screenshot 2024-07-23 at 14 32 24" src="https://github.com/user-attachments/assets/27358e1f-c4c3-4d9a-8e02-afae2cb4b682">

```cpp
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // Infinite range (be careful!)
    auto infinite = std::views::iota(1) | std::views::filter([](int n) { return n % 2 == 0; });
    
    // Take only the first 5 elements
    for (int i : infinite | std::views::take(5)) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    
    // Using common view for legacy interfaces
    auto filtered = vec | std::views::filter([](int n) { return n % 2 == 0; })
                       | std::views::common;
    
    // Legacy function expecting begin/end
    auto legacy_sum = [](auto begin, auto end) {
        return std::accumulate(begin, end, 0);
    };
    
    std::cout << "Sum of even numbers: " << legacy_sum(filtered.begin(), filtered.end()) << '\n';
}
```



# C++23 Views and Ranges Improvements

C++23 brings several enhancements and additions to the Ranges library, building upon the foundation laid in C++20. These improvements make working with ranges and views more convenient, expressive, and powerful. Here's a comprehensive overview of the changes:

## 1. New View Adaptors

### 1.1 `views::chunk_by`

`views::chunk_by` allows you to chunk a range based on a predicate, rather than a fixed size.

```cpp
std::vector<int> v{1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4};
auto chunks = v | std::views::chunk_by(std::ranges::equal_to{});
// Result: {{1, 1, 1}, {2, 2}, {3, 3, 3, 3}, {4, 4}}
```

### 1.2 `views::slide`

`views::slide` creates a sliding window view over a range.

```cpp
std::vector<int> v{1, 2, 3, 4, 5};
auto windows = v | std::views::slide(3);
// Result: {{1, 2, 3}, {2, 3, 4}, {3, 4, 5}}
```

### 1.3 `views::stride`

`views::stride` creates a view that steps through the range by a specified amount.

```cpp
std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
auto strided = v | std::views::stride(3);
// Result: {1, 4, 7, 10}
```

### 1.4 `views::chunk`

While `views::chunk` was introduced in C++20, C++23 enhances it to work with sizes that don't evenly divide the range length.

```cpp
std::vector<int> v{1, 2, 3, 4, 5};
auto chunks = v | std::views::chunk(2);
// Result: {{1, 2}, {3, 4}, {5}}
```

### 1.5 `views::as_rvalue`

`views::as_rvalue` creates a view that yields rvalue references to the elements of the underlying range.

```cpp
std::vector<std::string> v{"hello", "world"};
auto rvalue_view = v | std::views::as_rvalue;
// Elements accessed through rvalue_view are rvalue references
```

## 2. Enhancements to Existing Views

### 2.1 `views::zip`

`views::zip` now supports arbitrary numbers of ranges, not just pairs.

```cpp
std::vector<int> v1{1, 2, 3};
std::vector<char> v2{'a', 'b', 'c'};
std::vector<double> v3{1.1, 2.2, 3.3};
auto zipped = std::views::zip(v1, v2, v3);
// Result: {{1, 'a', 1.1}, {2, 'b', 2.2}, {3, 'c', 3.3}}
```

### 2.2 `views::enumerate`

`views::enumerate` is now part of the standard library, creating pairs of indices and elements.

```cpp
std::vector<std::string> v{"hello", "world"};
auto enumerated = v | std::views::enumerate;
// Result: {{0, "hello"}, {1, "world"}}
```

## 3. New Range Adaptors

### 3.1 `ranges::to`

`ranges::to` allows easy conversion of ranges to containers.

```cpp
std::vector<int> v{1, 2, 3, 4, 5};
auto result = v 
    | std::views::filter([](int i) { return i % 2 == 0; })
    | std::ranges::to<std::vector>();
// result is std::vector<int>{2, 4}
```

### 3.2 `ranges::join_with`

`ranges::join_with` joins a range of ranges, inserting a specified element between them.

```cpp
std::vector<std::vector<int>> v{{1, 2}, {3, 4}, {5, 6}};
auto joined = v | std::ranges::join_with(0);
// Result: {1, 2, 0, 3, 4, 0, 5, 6}
```

## 4. Improvements to Range Algorithms

### 4.1 `ranges::fold_left` and `ranges::fold_right`

These algorithms perform left and right folds over a range.

```cpp
std::vector<int> v{1, 2, 3, 4, 5};
auto sum = std::ranges::fold_left(v, 0, std::plus{});
// sum is 15
```

### 4.2 `ranges::starts_with` and `ranges::ends_with`

These algorithms check if a range starts or ends with a specific subsequence.

```cpp
std::vector<int> v{1, 2, 3, 4, 5};
bool starts = std::ranges::starts_with(v, std::vector{1, 2});  // true
bool ends = std::ranges::ends_with(v, std::vector{4, 5});      // true
```

## 5. Concept Improvements

### 5.1 `std::range_adaptor_closure`

This concept is introduced to formalize range adaptor closures, enabling better composition of range adaptors.

```cpp
template<typename T>
concept range_adaptor_closure = ...;  // Actual definition is more complex
```

## 6. Performance Improvements

C++23 includes various performance optimizations for ranges and views, including:

- More efficient implementations of certain view adaptors
- Improved compile-time performance for range-based for loops
- Better optimization opportunities for range-based algorithms

## Conclusion

C++23 significantly enhances the Ranges library, providing more powerful tools for working with sequences of data. These improvements allow for more expressive, efficient, and flexible code when dealing with collections and data streams. The new features build upon the solid foundation laid in C++20, further cementing ranges and views as core components of modern C++ programming.
