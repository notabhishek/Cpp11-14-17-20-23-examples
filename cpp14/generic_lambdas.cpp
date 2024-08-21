#include <iostream>

int main() {
    auto lambda = [](auto x, auto y) { return x + y; };
    std::cout << lambda(1, 2) << std::endl;
    std::cout << lambda(1.5, 2.7) << std::endl;
    return 0;
}


