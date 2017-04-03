#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

TEST(Group, Divisible) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4, 5};
    auto s = MakeStream(vec);
    auto &s_g = s.group(2);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_g.nth(i/2).nth(i%2));
    }
}

// TODO: fix me: error that s is different object than s_g.
#if 0
TEST(Group, Divisible) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4, 5};
    auto s = MakeStream(vec);
    auto s_g = s.group(2);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_g.nth(i/2).nth(i%2));
    }
}
#endif

TEST(Group, WithReminder) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec);
    auto &s_g = s.group(2);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_g.nth(i/2).nth(i%2));
    }
}

// TODO: fix me: error that s is different object than s_g.
#if 0
TEST(Group, WithReminder) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec);
    auto s_g = s.group(2);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_g.nth(i/2).nth(i%2));
    }
}
#endif
