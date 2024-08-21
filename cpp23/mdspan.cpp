#include <iostream>
#include <mdspan>
#include <vector>

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    auto matrix = std::mdspan(data.data(), 2, 3);
    
    for (std::size_t i = 0; i < matrix.extent(0); ++i) {
        for (std::size_t j = 0; j < matrix.extent(1); ++j) {
            std::cout << matrix[i, j] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}


