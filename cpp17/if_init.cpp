// File: cpp17/if_init.cpp
#include <iostream>
#include <map>

int main() {
    std::map<std::string, int> m = {{"hello", 42}};
    if (auto it = m.find("hello"); it != m.end()) {
        std::cout << "Found: " << it->second << std::endl;
    }
    return 0;
}