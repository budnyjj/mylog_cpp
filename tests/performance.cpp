#include <chrono>
#include <functional>
#include <iomanip>

#include "gtest/gtest.h"

#include "logging.hpp"


using std::function;


namespace {

std::chrono::duration<int32_t, std::micro>
measure(function<void()> predicate) {
    const auto begin = std::chrono::high_resolution_clock::now();
    predicate();
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - begin
    );
}

} // namespace


TEST(Basic, PerformanceGuard) {
    const auto num_iters = 1000;
    const auto log_duration = measure([]() {
        for (auto i = 0; i < num_iters; ++i) {
            d() << "test string";
        }
    });
    const auto printf_duration = measure([]() {
        for (auto i = 0; i < num_iters; ++i) {
            printf("test string");
        }
    });
    const auto cout_duration = measure([]() {
        for (auto i = 0; i < num_iters; ++i) {
            std::cout << printf("test string");
        }
    });

    EXPECT_EQ(
        log_duration.count(),
        printf_duration.count());
    EXPECT_EQ(
        log_duration.count(),
        cout_duration.count());
}
