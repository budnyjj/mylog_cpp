#include <cstdio>
#include <iostream>

#include "liblog/log.hpp"


using std::size_t;


const size_t NUM_TEST_ITERS = 1000;
const char* const TEST_CHARS = "test string ";
const int TEST_INT = 42;
const float TEST_FLOAT = 0.123;


class A {
    std::size_t length;

    static const std::size_t CAPACITY = 128;
    static const std::size_t MAX_LENGTH = CAPACITY - 1;
};


void testPrintf() {
    printf("%s %d %f", TEST_CHARS, TEST_INT, TEST_FLOAT);
}

void testStdCout() {
    std::cout << TEST_CHARS << TEST_INT << TEST_FLOAT;
}

void testLog() {
    log::l() << TEST_CHARS << TEST_INT << TEST_FLOAT;
}

void testAux() {
    // return A();
}


int main() {
    for (size_t i = 0; i < NUM_TEST_ITERS; ++i) {
        testPrintf();
        testStdCout();
        testLog();
    }

    return 0;
}
