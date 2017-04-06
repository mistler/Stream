#include "gtest/gtest.h"

#include "src/stream.hpp"
#include <iostream>

using namespace stream;

TEST(StreamTest, SimpleTest) {
    auto s = MakeStream({1, 2, 3, 4, 5});
    (s | map([](int x) { return x * x; }) | print_to(std::cout)) << std::endl;
}

TEST(StreamTest, ComplexTest) {
    float f = MakeStream(1, 2, 3)
        | map([](auto x) { return static_cast<float>(x * x); })
        | map([](auto x) { return std::vector<float>{x}; })
        | map([](auto x) { return x[0]; })
        | reduce([](float x, float y) { return x+y; });
    EXPECT_NEAR(14.f, f, 1e-5);
}
