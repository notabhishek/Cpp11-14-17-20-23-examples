// File: cpp17/nested_namespaces.cpp
#include <iostream>

namespace A::B::C {
    void foo() { std::cout << "A::B::C::foo()" << std::endl; }
}

int main() {
    A::B::C::foo();
    return 0;
}