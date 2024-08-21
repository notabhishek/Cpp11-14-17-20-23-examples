// File: cpp17/filesystem.cpp
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path p = std::filesystem::current_path();
    std::cout << "Current path: " << p << std::endl;
    return 0;
}