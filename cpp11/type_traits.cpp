#include <iostream>
#include <type_traits>

template <typename T>
void foo(T t) {
    if (std::is_integral<T>::value) {
        std::cout << "T is integral" << std::endl;
    } else {
        std::cout << "T is not integral" << std::endl;
    }
}

int main() {
    foo(10);
    foo(10.5);
    return 0;
}


