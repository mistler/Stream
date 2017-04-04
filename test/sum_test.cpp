#include "gtest/gtest.h"

#include "src/stream.hpp"

using namespace stream;

TEST(Sum, IntSimple) {
    auto s = MakeStream({0, 1, 2, 3, 4});
    EXPECT_EQ(0+1+2+3+4, s.sum());
}

TEST(Sum, PipeIntSimple) {
    auto s = MakeStream({0, 1, 2, 3, 4}) | sum();
    EXPECT_EQ(0+1+2+3+4, s);
}
