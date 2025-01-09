#include <iostream>
#include <generator>
#include <ranges>
std::generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        int temp = a;
        a = b;
        b = temp + b;
    }
}

int main() {
    auto fib = fibonacci();
    for (size_t idx = 10; int i : fib) {
        std::cout << i << " ";
        if(--idx <= 0) {
            break;
        }
    }
    std::cout << std::endl;
    return 0;
}

