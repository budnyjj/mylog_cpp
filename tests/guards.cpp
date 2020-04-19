#include <memory>

#include "gtest/gtest.h"

#include "liblog/log.hpp"


using log::Buffer;
using log::Logger;
using std::unique_ptr;


TEST(Basic, SizeGuard) {
    EXPECT_EQ(
        sizeof(Logger),
        sizeof(unique_ptr<void>)
            + Buffer::STATIC_SIZE
            + sizeof(Buffer::size_t));
}
