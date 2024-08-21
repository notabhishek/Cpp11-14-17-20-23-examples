// File: cpp17/string_view.cpp
#include <iostream>
#include <string_view>

void print(std::string_view sv) {
    std::cout << sv << std::endl;
}

int main() {
    print("Hello, world!");
    return 0;
}
