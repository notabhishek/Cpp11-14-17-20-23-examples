# C++20 Spans: A Comprehensive Guide

## 1. Introduction to Spans

`std::span` is a non-owning view over a contiguous sequence of objects. It provides a safe and efficient way to work with arrays and array-like data structures.

### 1.1 Basic Usage

```cpp
#include <span>
#include <vector>
#include <iostream>

void printSpan(std::span<const int> s) {
    for (int i : s) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> vec = {6, 7, 8, 9, 10};

    printSpan(arr);
    printSpan(vec);
}
```

## 2. Dynamic and Fixed Extents

Spans can have either a dynamic extent (size known at runtime) or a fixed extent (size known at compile-time).

```cpp
#include <span>
#include <iostream>

void printFixedSpan(std::span<const int, 5> s) {
    for (int i : s) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    std::span<int> dynamicSpan(arr);
    std::span<int, 5> fixedSpan(arr);

    std::cout << "Dynamic extent size: " << dynamicSpan.size() << '\n';
    std::cout << "Fixed extent size: " << fixedSpan.size() << '\n';

    printFixedSpan(arr);
    // printFixedSpan(dynamicSpan); // Error: cannot convert dynamic extent to fixed extent
}
```

## 3. Span Operations

Spans provide various operations to work with the underlying data.

```cpp
#include <span>
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::span s(arr);

    std::cout << "First element: " << s.front() << '\n';
    std::cout << "Last element: " << s.back() << '\n';
    std::cout << "Element at index 3: " << s[3] << '\n';

    auto firstHalf = s.first(5);
    auto lastHalf = s.last(5);
    auto middle = s.subspan(2, 6);

    std::cout << "First half: ";
    for (int i : firstHalf) std::cout << i << ' ';
    std::cout << '\n';

    std::cout << "Last half: ";
    for (int i : lastHalf) std::cout << i << ' ';
    std::cout << '\n';

    std::cout << "Middle: ";
    for (int i : middle) std::cout << i << ' ';
    std::cout << '\n';
}
```

## 4. Using Spans with Algorithms

Spans can be used with standard algorithms, providing a convenient way to work with contiguous sequences.

```cpp
#include <span>
#include <algorithm>
#include <iostream>

int main() {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::span s(arr);

    std::sort(s.begin(), s.end());

    std::cout << "Sorted array: ";
    for (int i : s) std::cout << i << ' ';
    std::cout << '\n';

    auto it = std::find(s.begin(), s.end(), 7);
    if (it != s.end()) {
        std::cout << "Found 7 at index: " << std::distance(s.begin(), it) << '\n';
    }

    std::reverse(s.begin(), s.end());

    std::cout << "Reversed array: ";
    for (int i : s) std::cout << i << ' ';
    std::cout << '\n';
}
```

## 5. Span as a Function Parameter

Using spans as function parameters provides a flexible and efficient way to accept different types of contiguous sequences.

```cpp
#include <span>
#include <vector>
#include <array>
#include <iostream>

void processData(std::span<const int> data) {
    int sum = 0;
    for (int i : data) {
        sum += i;
    }
    std::cout << "Sum: " << sum << '\n';
    std::cout << "Average: " << static_cast<double>(sum) / data.size() << '\n';
}

int main() {
    int cArray[] = {1, 2, 3, 4, 5};
    std::vector<int> vec = {6, 7, 8, 9, 10};
    std::array<int, 5> stdArray = {11, 12, 13, 14, 15};

    processData(cArray);
    processData(vec);
    processData(stdArray);
}
```

## 6. Spans and Multidimensional Arrays

Spans can be used to work with multidimensional arrays more easily.

```cpp
#include <span>
#include <iostream>

void print2DArray(std::span<const int, 12> arr, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << arr[i * cols + j] << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    int arr2D[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    print2DArray(std::span{&arr2D[0][0], 12}, 3, 4);
}
```

## 7. Performance Considerations

Spans are designed to be zero-overhead abstractions, providing safety without compromising performance.

```cpp
#include <span>
#include <vector>
#include <chrono>
#include <iostream>

void sumWithPointer(const int* arr, size_t size) {
    long long sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += arr[i];
    }
}

void sumWithSpan(std::span<const int> arr) {
    long long sum = 0;
    for (int i : arr) {
        sum += i;
    }
}

int main() {
    std::vector<int> vec(1'000'000);
    for (int i = 0; i < 1'000'000; ++i) {
        vec[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    sumWithPointer(vec.data(), vec.size());
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Pointer version: " << duration.count() << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    sumWithSpan(vec);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Span version: " << duration.count() << " microseconds\n";
}
```

## 8. Best Practices and Pitfalls

1. Use spans instead of pointer and size pairs for improved safety and clarity.
2. Prefer `std::span<const T>` for read-only access to improve const-correctness.
3. Be cautious of dangling spans when the underlying container is modified or goes out of scope.
4. Use fixed-extent spans when the size is known at compile-time for additional type safety.

```cpp
#include <span>
#include <vector>
#include <iostream>

std::span<const int> getDanglingSpan() {
    std::vector<int> temp = {1, 2, 3, 4, 5};
    return std::span(temp); // Dangerous: returns a span to a vector that will be destroyed
}

int main() {
    // Good practice: use const span for read-only access
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::span<const int> constSpan(vec);

    // Pitfall: modifying the underlying container can invalidate the span
    std::span<int> mutableSpan(vec);
    vec.push_back(6); // This may invalidate mutableSpan

    // Good practice: use fixed-extent span when size is known
    int arr[5] = {1, 2, 3, 4, 5};
    std::span<int, 5> fixedSpan(arr);

    // Pitfall: dangling span
    auto danglingSpan = getDanglingSpan(); // Dangerous: danglingSpan refers to destroyed data
    // std::cout << danglingSpan[0] << '\n'; // Undefined behavior

    // Good practice: use span to simplify function interfaces
    auto printSpan = [](std::span<const int> s) {
        for (int i : s) std::cout << i << ' ';
        std::cout << '\n';
    };

    printSpan(vec);
    printSpan(arr);
}
```


## 9. C++23 introduces mdspan
<img width="626" alt="Screenshot 2024-07-23 at 14 36 06" src="https://github.com/user-attachments/assets/8c8691aa-0355-46bc-a00c-f1f198444a65">



