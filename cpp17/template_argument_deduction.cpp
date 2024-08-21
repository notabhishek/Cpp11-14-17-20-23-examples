// File: cpp17/template_argument_deduction.cpp
#include <iostream>
#include <vector>

template<typename T>
class MyClass {
public:
    MyClass(T t) : value(t) {}
    T value;
};

int main() {
    MyClass c(42); // C++17: No need to specify <int>
    std::cout << c.value << std::endl;
    return 0;
}