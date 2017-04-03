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

TEST(Group, DivisibleDifferentObject) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4, 5};
    auto s = MakeStream(vec);
    auto s_g = s.group(2);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_g.nth(i/2).nth(i%2));
    }
}

TEST(Group, WithReminder) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec);
    auto &s_g = s.group(2);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_g.nth(i/2).nth(i%2));
    }
}

TEST(Group, WithReminderDifferentObject) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec);
    auto s_g = s.group(2);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_g.nth(i/2).nth(i%2));
    }
}
