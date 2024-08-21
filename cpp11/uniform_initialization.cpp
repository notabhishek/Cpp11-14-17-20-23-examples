#include <iostream>

struct Point {
    int x, y;
};

int main() {
    Point p{10, 20};
    std::cout << p.x << ", " << p.y << std::endl;
    return 0;
}


