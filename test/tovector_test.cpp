#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

TEST(ToVector, SimpleCompare) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    std::vector<int> s_vec = s.to_vector();
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_vec[i]);
    }
}

TEST(ToVector, PipeSimpleCompare) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end()) | to_vector();
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s[i]);
    }
}
