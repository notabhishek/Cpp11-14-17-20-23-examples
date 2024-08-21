#include <iostream>
#include <generator>

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
    for (int i = 0; i < 10; ++i) {
        std::cout << fib() << " ";
    }
    std::cout << std::endl;
    return 0;
}


