#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (auto i : v) std::cout << i << " ";
    std::cout << std::endl;
    return 0;
}


