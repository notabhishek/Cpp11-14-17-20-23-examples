// File: cpp17/optional.cpp
#include <iostream>
#include <optional>

std::optional<int> divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

int main() {
    if (auto result = divide(10, 2); result) {
        std::cout << "Result: " << *result << std::endl;
    }
    return 0;
}