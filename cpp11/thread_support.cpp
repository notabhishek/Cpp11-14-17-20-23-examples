#include <iostream>
#include <thread>

void foo() {
    std::cout << "Hello from thread" << std::endl;
}

int main() {
    std::thread t(foo);
    t.join();
    return 0;
}


