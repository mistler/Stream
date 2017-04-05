#include "gtest/gtest.h"

#include "src/stream.hpp"
#include <iostream>

using namespace stream;

TEST(StreamTest, SimpleTest) {
    auto s = MakeStream({1, 2, 3, 4, 5});
    (s | map([](int x) { return x * x; }) | print_to(std::cout)) << std::endl;
}
