#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

TEST(Nth, CompareWithVector) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s.nth(i));
    }
}
