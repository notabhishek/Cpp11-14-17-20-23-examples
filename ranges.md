# C++20 Ranges: A Comprehensive Guide

## 1. Introduction to Ranges

Ranges are a powerful feature introduced in C++20 that provide a more intuitive and composable way to work with sequences of elements. They offer lazy evaluation, improved readability, and enhanced performance in many scenarios.

### 1.1 Basic Range Concepts

```cpp
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // Basic range-based for loop
    for (int i : vec) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    
    // Using a view
    auto even_numbers = vec | std::views::filter([](int n) { return n % 2 == 0; });
    for (int i : even_numbers) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}
```

## 2. Range Adaptors

Range adaptors allow you to create new views by applying operations to existing ranges.

### 2.1 Common Range Adaptors

```cpp
#include <ranges>
#include <vector>
#include <iostream>
#include <string>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Filter
    auto even = vec | std::views::filter([](int n) { return n % 2 == 0; });
    
    // Transform
    auto squared = even | std::views::transform([](int n) { return n * n; });
    
    // Take
    auto first_three = squared | std::views::take(3);
    
    for (int i : first_three) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    
    // Reverse
    std::string s = "Hello, World!";
    auto reversed = s | std::views::reverse;
    std::cout << std::string(reversed.begin(), reversed.end()) << '\n';
}
```

## 3. Lazy Evaluation

Ranges provide lazy evaluation, meaning operations are only performed when the results are actually needed.

```cpp
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec(1'000'000);
    std::iota(vec.begin(), vec.end(), 1);
    
    auto result = vec 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; })
        | std::views::take(5);
    
    // Only the first 5 even numbers are actually squared
    for (int i : result) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}
```

## 4. Creating Custom Views

You can create your own views to extend the functionality of ranges.

```cpp
#include <ranges>
#include <iostream>

template<std::ranges::input_range R>
class every_nth_element_view : public std::ranges::view_interface<every_nth_element_view<R>> {
    R base_;
    std::size_t n_;

    class iterator {
        // Iterator implementation
    };

public:
    every_nth_element_view(R base, std::size_t n) : base_(std::move(base)), n_(n) {}

    auto begin() { return iterator{std::ranges::begin(base_), std::ranges::end(base_), n_}; }
    auto end() { return iterator{std::ranges::end(base_), std::ranges::end(base_), n_}; }
};

template<std::ranges::input_range R>
auto every_nth_element(R&& r, std::size_t n) {
    return every_nth_element_view(std::forward<R>(r), n);
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i : every_nth_element(v, 3)) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}
```

## 5. Range Algorithms

C++20 introduces range-based versions of algorithms that work directly with ranges.

```cpp
#include <ranges>
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    
    // Sort the entire vector
    std::ranges::sort(vec);
    for (int i : vec) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    
    // Find the first element greater than 5
    auto it = std::ranges::find_if(vec, [](int n) { return n > 5; });
    if (it != vec.end()) {
        std::cout << "First element > 5: " << *it << '\n';
    }
    
    // Count elements less than 5
    auto count = std::ranges::count_if(vec, [](int n) { return n < 5; });
    std::cout << "Elements < 5: " << count << '\n';
}
```

## 6. Projections

Projections allow you to apply a transformation to elements before they are processed by an algorithm.

```cpp
#include <ranges>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;
};

int main() {
    std::vector<Person> people = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 20},
        {"David", 35}
    };
    
    // Sort by age
    std::ranges::sort(people, {}, &Person::age);
    
    for (const auto& person : people) {
        std::cout << person.name << ": " << person.age << '\n';
    }
    
    // Find person by name
    auto it = std::ranges::find(people, "Charlie", &Person::name);
    if (it != people.end()) {
        std::cout << "Found: " << it->name << ", age: " << it->age << '\n';
    }
}
```

## 7. Performance Considerations

While ranges can often lead to more efficient code due to lazy evaluation, it's important to be aware of potential performance implications.

```cpp
#include <ranges>
#include <vector>
#include <iostream>
#include <chrono>

int main() {
    std::vector<int> vec(1'000'000);
    std::iota(vec.begin(), vec.end(), 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Using ranges
    auto sum_ranges = std::accumulate(
        vec | std::views::filter([](int n) { return n % 2 == 0; })
             | std::views::transform([](int n) { return n * n; }),
        0
    );
    
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Ranges time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << "ms\n";
    
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
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << "ms\n";
    
    std::cout << "Sum (ranges): " << sum_ranges << '\n';
    std::cout << "Sum (traditional): " << sum_traditional << '\n';
}
```

## 8. Best Practices and Pitfalls

1. Use ranges to simplify complex algorithms and improve readability.
2. Be cautious of creating infinite ranges without proper termination conditions.
3. Remember that views are lazy; they don't perform any work until iterated.
4. Use `std::views::common` when interfacing with legacy code expecting begin/end pairs.

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

This comprehensive guide covers the key aspects of Ranges in C++20, including basic concepts, range adaptors, lazy evaluation, custom views, range algorithms, projections, performance considerations, and best practices. The examples provided demonstrate practical usage and potential pitfalls to watch out for when using these new features.