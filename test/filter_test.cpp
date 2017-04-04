#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

TEST(Filter, EvenNumbersPassByValue) {
    auto sv = MakeStream({0, 1, 2, 3, 4, 5});
    auto filt = [](int x) { return (x%2) == 0; };
    sv = sv.filter(filt);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(i*2, sv.nth(i));
    }
}

TEST(Filter, PipeEvenNumbersPassByValue) {
    auto filt = [](int x) { return (x%2) == 0; };
    auto sv = MakeStream({0, 1, 2, 3, 4, 5}) | filter(filt);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(i*2, sv.nth(i));
    }
}

TEST(Filter, EvenNumbersTemporaryObject) {
    auto sv = MakeStream({0, 1, 2, 3, 4, 5});
    sv = sv.filter([](int x) { return (x%2) == 0; });
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(i*2, sv.nth(i));
    }
}

TEST(Filter, PipeEvenNumbersTemporaryObject) {
    auto sv = MakeStream({0, 1, 2, 3, 4, 5}) |
        filter([](int x) { return (x%2) == 0; });
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(i*2, sv.nth(i));
    }
}
