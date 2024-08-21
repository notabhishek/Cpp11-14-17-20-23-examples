#include <iostream>
#include <utility>

enum class Color { Red, Green, Blue };

int main() {
    Color c = Color::Green;
    std::cout << "Underlying value: " << std::to_underlying(c) << std::endl;
    return 0;
}


