#include <iostream>
#include <optional>

std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

int main() {
    auto result = safe_divide(10, 2)
                    .and_then([](int n) { return safe_divide(n, 2); })
                    .transform([](int n) { return n * 2; })
                    .or_else([]() { return 0; });
    
    std::cout << "Result: " << result << std::endl;
    return 0;
}


