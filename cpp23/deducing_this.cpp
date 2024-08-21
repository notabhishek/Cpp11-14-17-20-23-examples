#include <iostream>

struct S {
    void foo(this S& self) {
        std::cout << "Non-const foo" << std::endl;
    }
    
    void foo(this const S& self) const {
        std::cout << "Const foo" << std::endl;
    }
};

int main() {
    S s;
    s.foo();  // Prints "Non-const foo"
    const S cs;
    cs.foo(); // Prints "Const foo"
    return 0;
}


