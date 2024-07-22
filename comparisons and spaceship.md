# Comparisons in C++20: A Comprehensive Overview

## Introduction

- C++20 introduces significant changes to comparison operations
- New operator: `<=>` (three-way comparison or "spaceship" operator)
- Fundamental shift in comparison semantics
- Aim: Reduce boilerplate code and potential for errors

## Comparisons in C++17 and Earlier

### Pre-C++20 Rules:

- Six comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- All operators treated equally by the language
- Relied on idioms to simplify implementation (e.g., `!=` defined in terms of `==`)
- Typically implemented `==` and `<` as primitives

### Problems with Pre-C++20 Approach:

1. Verbose implementation:

```cpp
struct Point {
    int x, y;
    
    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const {
        return !(*this == rhs);
    }
    bool operator<(const Point& rhs) const {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }
    bool operator>(const Point& rhs) const {
        return rhs < *this;
    }
    bool operator<=(const Point& rhs) const {
        return !(rhs < *this);
    }
    bool operator>=(const Point& rhs) const {
        return !(*this < rhs);
    }
};
```

2. Error-prone for complex types
3. Difficulty in supporting heterogeneous comparisons
4. `<` as a primitive is problematic for partial orders (e.g., floating-point comparisons)

## Key Concept: Primary and Secondary Operators

| Category   | Equality | Ordering |
|------------|----------|----------|
| Primary    | `==`     | `<=>`    |
| Secondary  | `!=`     | `<, >, <=, >=` |

- Primary operators can be reversed
- Secondary operators can be rewritten in terms of primary operators

## The Spaceship Operator: `<=>`

### Comparison Categories

1. `strong_ordering`: 
   - Total ordering with substitutability
   - Values: `strong_ordering::greater`, `strong_ordering::equal`, `strong_ordering::less`

2. `weak_ordering`:
   - Total ordering with equivalence classes
   - Values: `weak_ordering::greater`, `weak_ordering::equivalent`, `weak_ordering::less`

3. `partial_ordering`:
   - Partial ordering
   - Values: `partial_ordering::greater`, `partial_ordering::equivalent`, `partial_ordering::less`, `partial_ordering::unordered`

### Example: Implementing Partial Ordering

```cpp
struct IntNan {
    std::optional<int> val = std::nullopt;

    bool operator==(IntNan const& rhs) const {
        if (!val || !rhs.val) {
            return false;
        }
        return *val == *rhs.val;
    }

    std::partial_ordering operator<=>(IntNan const& rhs) const {
        if (!val || !rhs.val) {
            return std::partial_ordering::unordered;
        }
        return *val <=> *rhs.val;
    }
};

IntNan{2} <=> IntNan{4};  // partial_ordering::less
IntNan{2} <=> IntNan{};   // partial_ordering::unordered
IntNan{2} < IntNan{4};    // true
IntNan{2} < IntNan{};     // false
IntNan{2} == IntNan{};    // false
IntNan{2} <= IntNan{};    // false
```

## New Operator Abilities

### 1. Reversing Primary Operators

```cpp
struct A {
    int i;
    explicit A(int i) : i(i) { }

    bool operator==(int j) const {
        return i == j;
    }

    std::strong_ordering operator<=>(int j) const {
        return i <=> j;
    }
};

A a(10);
a == 10;    // OK: a.operator==(10)
10 == a;    // OK in C++20: a.operator==(10)
a <=> 10;   // OK: a.operator<=>(10)
10 <=> a;   // OK in C++20: 0 <=> a.operator<=>(10)
```

- No new functions generated
- Only primary operators are reversible

### 2. Rewriting Secondary Operators

```cpp
A a(10);
a != 17;    // Rewrites to !(a == 17)
a < 9;      // Rewrites to (a <=> 9) < 0
17 != a;    // Rewrites to !a.operator==(17)
9 <= a;     // Rewrites to 0 <= a.operator<=>(9)
```

## Specific Lookup Rules

1. Find all `operator@`s
2. Find all `operator@@`s (primary operator of @)
3. Consider reversed `operator@@`s
4. Perform single overload resolution run
5. Pick the best viable candidate

### Example: Ambiguity Resolution

```cpp
struct C {
    bool operator==(C const&) const;
    bool operator!=(C const&) const;
};

bool check(C x, C y) {
    return x != y;
}
```

In C++20, `x != y` has three candidates:
1. `x.operator!=(y)`
2. `!x.operator==(y)`
3. `!y.operator==(x)`

Tiebreaker rules:
1. Non-reversed candidates win over reversed ones
2. Non-rewritten candidates win over rewritten ones

Result: `x.operator!=(y)` is chosen

## Summary of Rules

| Source | Alt 1 | Alt 2 |
|--------|-------|-------|
| `a == b` | `b == a` | |
| `a != b` | `!(a == b)` | `!(b == a)` |
| `a <=> b` | `0 <=> (b <=> a)` | |
| `a < b` | `(a <=> b) < 0` | `(b <=> a) > 0` |
| `a <= b` | `(a <=> b) <= 0` | `(b <=> a) >= 0` |
| `a > b` | `(a <=> b) > 0` | `(b <=> a) < 0` |
| `a >= b` | `(a <=> b) >= 0` | `(b <=> a) <= 0` |

## Guideline: PRIMARY-ONLY

Define only the primary operators (`==` and `<=>`) for your type.

Benefits:
- Fewer operators to implement (2 instead of 6 for homogeneous, 2 instead of 12 for each heterogeneous)
- Full complement of comparisons
- Can be member functions

Example:

```cpp
struct Point {
    int x, y;

    bool operator==(const Point&) const = default;
    auto operator<=>(const Point&) const = default;
};

// All comparison operators now work:
Point p1{1, 2}, p2{3, 4};
p1 == p2;  // false
p1 != p2;  // true
p1 < p2;   // true
p1 <= p2;  // true
p1 > p2;   // false
p1 >= p2;  // false
```

## Defaulting Comparisons

### C++20 Approach:

```cpp
struct A {
    T t;
    U u;
    V v;

    auto operator<=>(const A&) const = default;
};
```

- Generates both `==` and `<=>` with member-wise lexicographical comparison
- Simplifies implementation
- Reduces potential for errors

### Example: Complex Number Comparison

```cpp
struct Complex {
    double real, imag;

    auto operator<=>(const Complex& rhs) const {
        if (auto cmp = real <=> rhs.real; cmp != 0) {
            return cmp;
        }
        return imag <=> rhs.imag;
    }

    bool operator==(const Complex& rhs) const = default;
};

Complex c1{1.0, 2.0}, c2{1.0, 3.0};
c1 < c2;   // true
c1 == c2;  // false
```

## Heterogeneous Comparisons

C++20 makes it easier to support heterogeneous comparisons:

```cpp
struct String {
    std::string s;

    bool operator==(const char* rhs) const {
        return s == rhs;
    }

    std::strong_ordering operator<=>(const char* rhs) const {
        return s <=> rhs;
    }
};

String str{"hello"};
str == "hello";     // true
str < "world";      // true
"hello" == str;     // true (reversed)
"world" > str;      // true (reversed and rewritten)
```

## Conclusion

- C++20 brings significant improvements to comparison operations
- New `<=>` operator simplifies ordering comparisons
- Reversible primary operators and rewritable secondary operators reduce boilerplate
- Defaulted comparisons further simplify common cases
- Support for heterogeneous comparisons is greatly improved
- Overall, more intuitive and less error-prone comparison logic


## Further reading
- https://en.cppreference.com/w/cpp/language/default_comparisons
- https://github.com/notabhishek/cpp20/blob/main/C%2B%2B20_cpponsea_240630.pdf
- https://brevzin.github.io/c++/2019/07/28/comparisons-cpp20/
