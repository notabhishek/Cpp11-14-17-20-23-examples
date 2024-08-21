#include <iostream>

void foo(int&& x) {
    std::cout << "rvalue reference: " << x << std::endl;
}

int main() {
    foo(10);  // 10 is an rvalue
    return 0;
}


