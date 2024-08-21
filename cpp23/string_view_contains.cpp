#include <iostream>
#include <string_view>

int main() {
    std::string_view sv = "Hello, world!";
    
    std::cout << "Contains world: " << sv.contains("world") << std::endl;
    std::cout << "Contains World: " << sv.contains("World") << std::endl;
    
    return 0;
}


