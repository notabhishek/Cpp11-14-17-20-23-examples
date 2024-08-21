#include <iostream>
#include <vector>
#include <string>

constexpr std::vector<int> create_vector() {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    return v;
}

constexpr std::string create_string() {
    std::string s = "Hello";
    s += ", world!";
    return s;
}

int main() {
    constexpr auto v = create_vector();
    constexpr auto s = create_string();
    
    std::cout << "Vector size: " << v.size() << std::endl;
    std::cout << "String: " << s << std::endl;
    
    return 0;
}


