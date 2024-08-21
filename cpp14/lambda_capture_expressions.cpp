#include <iostream>

int main() {
    int x = 4;
    auto lambda = [y = x + 1] { return y * y; };
    std::cout << lambda() << std::endl;
    return 0;
}


