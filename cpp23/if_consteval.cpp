#include <iostream>

constexpr int factorial(int n) {
    if consteval {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    } else {
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
}

int main() {
    constexpr int compile_time_result = factorial(5);
    std::cout << "Compile-time result: " << compile_time_result << std::endl;

    int n = 5;
    int runtime_result = factorial(n);
    std::cout << "Runtime result: " << runtime_result << std::endl;

    return 0;
}


