#include <iostream>

void foo(int* ptr) {
    if (ptr == nullptr) std::cout << "null pointer" << std::endl;
    else std::cout << "not null" << std::endl;
}

int main() {
    foo(nullptr);
    return 0;
}


