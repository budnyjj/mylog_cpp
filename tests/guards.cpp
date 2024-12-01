#include <memory>

#include "gtest/gtest.h"

#include "logging.hpp"


using my::log::Buffer;
using my::log::Level;
using my::log::Logger;
using std::unique_ptr;


TEST(Basic, DebugPutsSizeGuard) {
    EXPECT_EQ(
        sizeof(Logger<Level::DEBUG, PutsWriter>),
        sizeof(unique_ptr<void>)
            + Buffer::STATIC_SIZE
            + sizeof(Buffer::size_t)
    );
}
