#include <iostream>
#include <random>

int main() {
    // Old version
    srand(time(nullptr));
    std::cout << rand() % 9 + 1 << std::endl;

    // New version - needs to include random
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 9);
    std::cout << dist(rd) << std::endl;

    return 0;
}
