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

TEST(Skip, PipeSkip) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    constexpr int skip_s = 2;
    const int vlen = vec.size();
    auto s = MakeStream(vec.begin(), vec.end()) | skip(skip_s);
    for (int i = skip_s; i < vlen; i++) {
        EXPECT_EQ(vec[i], s.nth(i-skip_s));
    }
}
