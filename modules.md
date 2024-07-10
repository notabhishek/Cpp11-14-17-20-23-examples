# C++20 Features: Modules

## Introduction to Modules

### Why Modules?

- **Scaling C++ with Components:**
  - Designed to handle codebases with over 1,000,000,000 lines of code (LoC).
  - Due to templates, almost everything is traditionally in header files.

- **Key Approach:**
  - Combine multiple translation units (header and source files) into one component.

### Benefits of Modules

- **Compile-Time Savings:**
  - Significant reduction in compile times since many codes are parsed and compiled only once (if not inlined).

- **No Runtime Overhead:**
  - Unlike some other optimizations, modules do not add runtime costs.

- **Clear Abstraction and Information Hiding:**
  - Better encapsulation and control over what is exposed to other parts of the program.

## Header Files vs. Modules

### Traditional Header Files

```cpp
#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#define SAFE_MODE
#include <iostream>

class Type {
    // ...
};

template<typename T>
Type toType(const T&) {
    // ...
}

namespace myclassintern {
    inline void init(Type&) {
        // ...
    }
}

#endif // MYCLASS_HPP
```

- **Drawbacks:**
  - Everything is visible.
  - Code is compiled multiple times.

### Modules

```cpp
module;

#define SAFE_MODE
#include <iostream>

export module Type;  // declare module "Type"

export class Type {
    // ...
};

export template<typename T>
Type toType(const T&) {
    // ...
}

void init(Type&) {
    // ...
}
```

- **Advantages:**
  - Only exported symbols are visible.
  - Pre-compiled code in non-portable format (e.g., Type.gcm, Type.ifc, or Type.bmi).

### Usage Comparison

#### Traditional Inclusion

```cpp
#include "MyClass.hpp"

Type x = toType(42);
```

#### Modern Import

```cpp
import Type;

Type x = toType(42);
```

- **Import:**
  - Makes only exports visible.
  - Symbols are visible with `import`.

## Example: Primary Interface and Usage

### Primary Interface

```cpp
export module Prime; // primary interface of Prime

export bool isPrime(int value) {
    for (int i = 2; i <= value / 2; ++i) {
        if (value % i == 0) {
            return false;
        }
    }
    return value > 1;  // 0 and 1 are not prime numbers
}
```

### Usage in Main Program

```cpp
#include <iostream>
import Prime;

int main() {
    int count = 20;
    std::cout << "first " << count << " prime numbers:\n";
    int num = 0;
    for (int val = 1; num < count; ++val) {
        if (isPrime(val)) {
            std::cout << "  " << val << '\n';
            ++num;
        }
    }
}
```

### Compilation Process

1. **Pre-compile the Module:**
   - Generates a file in a non-portable format (e.g., Prime.ifc, gcm).

2. **Compile and Link:**
   - Import the module in other translation units.
   - Compile and link as usual.

## Summary

- **Modules in C++20:**
  - Provide a more scalable, efficient, and maintainable way to manage large codebases.
  - Reduce compile times and enhance encapsulation.
  - Offer a clear separation between interface and implementation.
