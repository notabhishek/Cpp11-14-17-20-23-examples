#include <iostream>
#include <stacktrace>

void foo() {
    std::cout << std::stacktrace::current() << std::endl;
}

int main() {
    foo();
    return 0;
}


