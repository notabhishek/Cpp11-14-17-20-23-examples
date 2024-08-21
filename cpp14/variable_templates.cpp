#include <iostream>

template<typename T>
constexpr T pi = T(3.1415926535897932385);

int main() {
    std::cout << pi<float> << std::endl;
    std::cout << pi<double> << std::endl;
    return 0;
}


