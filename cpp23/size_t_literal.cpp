#include <iostream>

int main() {
    auto size = 42uz; // uz is the new literal suffix for size_t
    std::cout << "Size: " << size << ", type: " << typeid(size).name() << std::endl;
    return 0;
}


