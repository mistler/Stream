#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

TEST(Skip, SimpleSkip) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    constexpr int skip = 2;
    const int vlen = vec.size();
    s = s.skip(skip);
    for (int i = skip; i < vlen; i++) {
        EXPECT_EQ(vec[i], s.nth(i-skip));
    }
}
