// File: cpp17/structured_bindings.cpp
#include <iostream>
#include <tuple>

int main() {
    auto [x, y, z] = std::make_tuple(1, 2.3, "hello");
    std::cout << x << ", " << y << ", " << z << std::endl;
    return 0;
}
