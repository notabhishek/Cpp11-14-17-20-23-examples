// File: cpp17/parallel_algorithms.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>

int main() {
    std::vector<int> v(1000000);
    std::iota(v.begin(), v.end(), 0);
    std::sort(std::execution::par, v.begin(), v.end());
    std::cout << "Sorted in parallel" << std::endl;
    return 0;
}