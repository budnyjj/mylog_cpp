#include <cstdio>
#include <iostream>

#include "logging.hpp"


using std::size_t;

constexpr size_t NUM_TEST_ITERS = 1000;
constexpr int TEST_INT = 42;
constexpr float TEST_FLOAT = 0.123;
const char* const TEST_CHARS = "test string";


int main() {
    for (size_t i = 0; i < NUM_TEST_ITERS; ++i) {
        D(TEST_CHARS << " " << TEST_INT << " " << TEST_FLOAT << " " << i);
    }
    return 0;
}
