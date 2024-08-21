// File: cpp17/variant.cpp
#include <iostream>
#include <variant>

int main() {
    std::variant<int, float, std::string> v = "hello";
    std::cout << std::get<std::string>(v) << std::endl;
    return 0;
}