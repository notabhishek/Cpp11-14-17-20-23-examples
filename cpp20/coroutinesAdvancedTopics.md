# C++20: Coroutines Advanced Topics

## Advanced Usage of Coroutines

### 1. Using Coroutines with Multiple Resumptions
Coroutines can be resumed multiple times, making them highly versatile. Here's an example demonstrating a coroutine that can be resumed until a specified maximum value is reached:

```cpp
CoroIF coro(int max) {
    std::cout << " beg coro" << max << "\n";

    // loop from 1 to max:
    for (int v = 1; v <= max; ++v) {
        std::cout << ' ' << v << '/' << max << '\n';
        co_await std::suspend_always(); // SUSPEND
    }

    std::cout << " end coro" << max << "\n";
}

int main() {
    std::cout << "START\n";
    CoroIF coroTask8 = coro(8);  // assume we start coroutines lazily
    CoroIF coroTask2 = coro(2);
    std::cout << "INIT DONE\n";

    std::cout << "RESUME8:\n";
    coroTask8.resume();          // RESUME coro(8)
    while (coroTask2.isResumable()) {
        std::cout << " RESUME2:\n";
        coroTask2.resume();      // RESUME coro(2)
    }

    std::cout << "RESUME8:\n";
    coroTask8.resume();          // RESUME coro(8)
    std::cout << "END\n";
}
```

**Output:**
```
START
INIT DONE
RESUME8:
 beg coro8
 1/8
 RESUME2:
 beg coro2
 1/2
 RESUME2:
 2/2
 RESUME2:
 end coro2
 RESUME8:
 2/8
END
```

### 2. Coroutines and Coroutine Frames
When a coroutine is started, a coroutine frame is created, usually on the heap, containing all the parameters and a promise to control its behavior.

**Key Points:**
- **Parameters and Promises**: Parameters are copied into the frame, and a promise is created to manage the coroutine's state and behavior.
- **Promise API**:
  - `co_await prm.initial_suspend();` - Initial suspension point.
  - `prm.yield_value(Type);` - Handling `co_yield`.
  - `prm.return_void();` and `prm.return_value(Type);` - Handling `co_return`.

Example implementation:
```cpp
Parameters params{...};  // copy of coroutine parameters 
PromiseType prm{...};    // promise type of the coroutine

// to execute the coroutine body:
try {
    co_await prm.initial_suspend();
    coroutine-body
}
catch (...) {
    if (!initial-await-resume-called) throw;
    prm.unhandled_exception();
}
co_await prm.final_suspend();
```

### 3. Issues with Coroutines
**Challenges:**
- **No Coroutine Interface Types Yet**: Many use cases and the need for standardized coroutine interfaces.
- **Memory Management**: Handling memory efficiently for coroutine frames.

### 4. Example of Coroutine with std::generator<>
C++23 will introduce `std::generator<>` for better coroutine interface handling.

Example:
```cpp
#include <generator>
...

std::generator<int> intValues(int val = 0) {
    while (true) {
        co_yield val++;
    }
}

int main() {
    for (auto i : intValues() | std::views::take(3)) {
        std::cout << i << ' ';  // prints 0 1 2
    }
}
```

### 5. Coroutines and Call-by-Reference
Be cautious with call-by-reference in coroutines to avoid runtime errors due to object lifetimes.

Example to avoid:
```cpp
template <typename T>
Gen<std::ranges::range_value_t<T>> nextElem(const T& coll) {
    for (auto elem : coll) {
        co_yield elem;  // yield value and suspend
    }
}

std::vector<int> getTemporaryColl();  // forward declaration

int main() {
    auto gen = nextElem(getTemporaryColl());
    // OOPS, returned vector destroyed here
    while (gen.resume()) {  // runtime ERROR
        std::cout << gen.getValue() << '\n';
    }
}
```

### Conclusion
Coroutines in C++20 provide powerful tools for creating asynchronous and concurrent code. However, they come with challenges like managing lifetimes and memory, which need careful consideration. Future enhancements in C++23 and C++26 promise to simplify and extend coroutine capabilities further.