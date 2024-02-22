#include <iostream>

#include "Triangle.hpp"

int main() {
    Triangle triangleApp;

    try {
        triangleApp.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}