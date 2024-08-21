# C++20 Concurrency Features

## Thread Synchronization Mechanisms
- **Latches**
  - Single-time synchronization points.
  - Multiple threads can wait for all to reach a point before proceeding.
- **Barriers**
  - Multiple-time synchronization points.
  - Threads repeatedly wait for others before proceeding, with optional callbacks.
- **Semaphores**
  - Lightweight primitives for limiting and synchronizing access to resources.
  - **Counting Semaphore**: Controls the number of threads accessing a resource.
  - **Binary Semaphore**: Acts as a simple on/off lock.
- **Output Sync Streams**
  - Ensures synchronized output from different threads.

## Example of `std::latch`
```cpp
#include <latch>
#include <vector>
#include <thread>
#include <array>
#include <iostream>
#include <chrono>

void exampleLatch() {
    std::array<char, 5> vals{'.', '?', '8', '-', '+'};
    std::latch allLoopsDone{vals.size()};

    std::vector<std::jthread> tasks;
    for (char c : vals) {
        tasks.push_back(std::jthread{[c, &allLoopsDone] {
            for (int i = 0; i < c; ++i) {
                std::cout.put(c).flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            allLoopsDone.count_down();
        }});
    }

    std::cout << "waiting until all tasks are done" << std::endl;
    allLoopsDone.wait();
    std::cout << "all loops done" << std::endl;
}
```

## Example of `std::barrier`
```cpp
#include <barrier>
#include <vector>
#include <thread>
#include <array>
#include <iostream>
#include <cmath>

void print(const std::array<double, 5>& vals) {
    for (double val : vals) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void compute() {
    std::array<double, 5> vals{1.0, 2.0, 3.0, 4.0, 5.0};
    std::barrier allRdy{vals.size(), [&]() noexcept {
        print(vals);
    }};

    std::vector<std::jthread> threads;
    for (std::size_t idx = 0; idx < vals.size(); ++idx) {
        threads.push_back(std::jthread{[idx, &vals, &allRdy](std::stop_token st) {
            while (!st.stop_requested()) {
                vals[idx] = std::sqrt(vals[idx]);
                allRdy.arrive_and_wait();
            }
            allRdy.arrive_and_drop();
        }});
    }
}
```

## Example of `std::counting_semaphore`
```cpp
#include <semaphore>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <iostream>

constexpr int numThreads = 10;
std::counting_semaphore<numThreads> maxParallel{0};

std::queue<int> values;
std::mutex valuesMx;

void processValues() {
    std::vector<std::jthread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::jthread{[&](std::stop_token st) {
            while (!st.stop_requested()) {
                maxParallel.acquire();
                {
                    std::lock_guard lock{valuesMx};
                    if (!values.empty()) {
                        int value = values.front();
                        values.pop();
                        std::cout << "Processing value: " << value << std::endl;
                    }
                }
                maxParallel.release();
            }
        }});
    }
    maxParallel.release(3); // Enable 3 threads initially
    for (int i = 0; i < 10; ++i) {
        {
            std::lock_guard lock{valuesMx};
            values.push(i);
        }
        maxParallel.release();
    }
}
```

## Example of `std::binary_semaphore`
```cpp
#include <semaphore>
#include <thread>
#include <iostream>

int inData;
std::binary_semaphore inReady{0};    // signal there is data to process 
std::binary_semaphore doneReady{0};  // signal processing is done

void process() {
    std::jthread processor{[&](std::stop_token st) {
        while (!st.stop_requested()) {
            if (inReady.try_acquire_for(std::chrono::seconds(1))) {
                int data = inData;
                std::cout << "Processing data: " << data << std::endl;
                doneReady.release();
            }
        }
    }};

    for (int i = 0; i < 10; ++i) {
        inData = i;
        inReady.release();
        doneReady.acquire();
    }
}
```

## Changes/Extensions for Atomic Types
- **Atomic Reference (`std::atomic_ref<>`)**
  - Temporary atomic interface to trivially copyable types.
- **Atomic Shared Pointer (`std::atomic<std::shared_ptr<>>`)**
  - Atomic operations for `std::shared_ptr<>`.
- **Specializations for Floating-Point Types**
  - Adds atomic support for addition and subtraction.
- **Thread Synchronization for All Atomic Types**
  - Using `wait()`, `notify_one()`, and `notify_all()`.

## Example of `std::atomic_ref`
```cpp
#include <atomic>
#include <array>
#include <vector>
#include <thread>
#include <iostream>
#include <random>

void exampleAtomicRef() {
    std::array<int, 1000> values;
    std::fill_n(values.begin(), values.size(), 100);

    std::vector<std::jthread> threads;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, values.size() - 1);

    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::jthread{[&values, &gen, &dis](std::stop_token st) {
            while (!st.stop_requested()) {
                int idx = dis(gen);
                std::atomic_ref<int> val{values[idx]};
                --val;
                if (val.load() <= 0) {
                    std::cout << "index " << idx << " is zero\n";
                }
            }
        }});
    }
}
```

## Example of Synchronized Output Streams
```cpp
#include <iostream>
#include <cmath>
#include <thread>
#include <syncstream>

void squareRoots(int num) {
    std::osyncstream coutSync{std::cout};
    for (int i = 0; i < num; ++i) {
        coutSync << "squareroot of " << i << " is " << std::sqrt(i) << '\n' << std::flush_emit;
    }
}

int main() {
    std::jthread t1(squareRoots, 5);
    std::jthread t2(squareRoots, 5);
    std::jthread t3(squareRoots, 5);
}
```

## Conclusion
- C++20 introduces powerful new tools for managing concurrency.
- Latches, barriers, semaphores, atomic types, and synchronized output streams enhance control over multi-threaded programs.
- These features simplify and enhance the safety of concurrent programming in C++.