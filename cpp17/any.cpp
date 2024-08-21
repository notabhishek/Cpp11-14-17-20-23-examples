// File: cpp17/any.cpp
#include <iostream>
#include <any>

int main() {
    std::any a = 42;
    std::cout << std::any_cast<int>(a) << std::endl;
    return 0;
}