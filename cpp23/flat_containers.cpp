#include <iostream>
#include <flat_map>

int main() {
    std::flat_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
    
    for (const auto& [key, value] : map) {
        std::cout << key << ": " << value << std::endl;
    }
    
    return 0;
}


