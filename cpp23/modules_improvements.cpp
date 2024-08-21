// Note: This is a conceptual example. Syntax and support may vary between compilers.
module;

#include <iostream>

export module my_module;

export void hello() {
    std::cout << "Hello from my_module!" << std::endl;
}

// In another file:
// import my_module;
// int main() {
//     hello();
//     return 0;
// }


