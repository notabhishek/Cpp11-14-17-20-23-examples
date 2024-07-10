# C++20 Coroutines

## Introduction to Coroutines
- **Overview**
  - Coroutines are generalized functions that can be paused and resumed.
  - They provide a powerful mechanism for writing asynchronous code.
  - Coroutines help in managing tasks without blocking threads.

## Key Components of Coroutines
- **Coroutine Handles**
  - `std::coroutine_handle<>` is a handle to a coroutine.
- **Coroutine Traits**
  - Determines the return type and promise type of coroutines.
- **Promise Object**
  - Manages the state of a coroutine and communicates between the coroutine and its caller.

## Basic Coroutine Example
```cpp
#include <coroutine>
#include <iostream>

struct ReturnObject {
    struct promise_type {
        ReturnObject get_return_object() { 
            return {}; 
        }
        std::suspend_never initial_suspend() { 
            return {}; 
        }
        std::suspend_never final_suspend() noexcept { 
            return {}; 
        }
        void unhandled_exception() { 
            std::terminate(); 
        }
    };
};

ReturnObject myCoroutine() {
    std::cout << "Hello from coroutine\n";
    co_return;
}

int main() {
    auto coro = myCoroutine();
    std::cout << "Hello from main\n";
    return 0;
}
```

## Awaitable Objects
- **Definition**
  - Objects that can be awaited in a coroutine.
  - Must implement three methods: `await_ready()`, `await_suspend()`, and `await_resume()`.

## Example of Awaitable Object
```cpp
#include <coroutine>
#include <iostream>
#include <thread>
#include <chrono>

struct Timer {
    std::chrono::milliseconds duration;
    Timer(std::chrono::milliseconds d) : duration(d) {}

    bool await_ready() const noexcept {
        return duration.count() == 0;
    }

    void await_suspend(std::coroutine_handle<> handle) const {
        std::thread([handle, duration = this->duration] {
            std::this_thread::sleep_for(duration);
            handle.resume();
        }).detach();
    }

    void await_resume() const noexcept {}
};

struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> coro;

    Task(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Task() { coro.destroy(); }
};

Task coroutineWithTimer() {
    std::cout << "Coroutine started\n";
    co_await Timer{std::chrono::seconds(2)};
    std::cout << "Coroutine resumed after 2 seconds\n";
}

int main() {
    coroutineWithTimer();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}
```

## Generator Coroutines
- **Overview**
  - Used to generate a sequence of values.
  - Uses `co_yield` to produce values.

## Example of Generator Coroutine
```cpp
#include <coroutine>
#include <iostream>
#include <vector>

template<typename T>
struct Generator {
    struct promise_type {
        T current_value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> coro;

    Generator(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Generator() { coro.destroy(); }

    struct iterator {
        std::coroutine_handle<promise_type> coro;
        bool done;

        iterator(std::coroutine_handle<promise_type> coro, bool done) : coro(coro), done(done) {}

        iterator& operator++() {
            coro.resume();
            done = coro.done();
            return *this;
        }

        T operator*() const { return coro.promise().current_value; }

        bool operator!=(const iterator& other) const {
            return done != other.done;
        }
    };

    iterator begin() {
        coro.resume();
        return iterator{coro, coro.done()};
    }

    iterator end() {
        return iterator{coro, true};
    }
};

Generator<int> generateNumbers(int max) {
    for (int i = 1; i <= max; ++i) {
        co_yield i;
    }
}

int main() {
    for (auto value : generateNumbers(5)) {
        std::cout << value << '\n';
    }
    return 0;
}
```

## Conclusion
- C++20 coroutines provide a powerful and flexible way to write asynchronous code.
- Key components include coroutine handles, promise objects, and awaitable objects.
- Generators and tasks are practical applications of coroutines.