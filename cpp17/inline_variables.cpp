// File: cpp17/inline_variables.cpp
#include <iostream>

inline int my_var = 42;

int main() {
    std::cout << my_var << std::endl;
    return 0;
}