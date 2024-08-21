#include <iostream>
#include <type_traits>

template <typename T>
void foo() {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
}

int main() {
    foo<int>();  // OK
    // foo<double>();  // Error
    return 0;
}


