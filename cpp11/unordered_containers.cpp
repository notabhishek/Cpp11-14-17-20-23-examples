#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
    std::cout << map["two"] << std::endl;
    return 0;
}


