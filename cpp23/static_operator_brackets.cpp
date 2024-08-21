#include <iostream>

struct MyArray {
    static constexpr int size = 3;
    int data[size] = {1, 2, 3};

    static constexpr int operator[](std::size_t i) {
        return i < size ? i + 1 : 0;
    }
};

int main() {
    constexpr MyArray arr;
    std::cout << MyArray::operator[](1) << std::endl; // Prints 2
    std::cout << arr[2] << std::endl; // Prints 3 (using the actual array data)
    return 0;
}


