#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <iostream>
#include <vector>
#include <memory>

using namespace stream;

TEST(UniquePtr, Simple) {
    auto s = MakeStream(std::unique_ptr<void*>(), std::unique_ptr<void*>());
    std::cout << ( s| map([](auto x) { return x; })
            | map([](auto x) { return x; }) | to_vector()).size() << std::endl;
}
