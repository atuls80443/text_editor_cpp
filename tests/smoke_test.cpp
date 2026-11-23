#include <iostream>

// Prints which C++ standard the compiler is using.
// 201703 means C++17, which is what we want.
int main() {
    std::cout << "Toolchain OK. C++ standard macro = " << __cplusplus << "\n";
    return 0;
}