#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

TEST(Reduce, IdentitySumSameType) {
    auto sv = MakeStream({0, 1, 2, 3, 4});
    auto total = sv.reduce([](int x) { return x; },
            [](int x, int y) { return x+y; });
    EXPECT_EQ(0+1+2+3+4, total);
}

TEST(Reduce, PipeIdentitySumSameType) {
    auto total = MakeStream({0, 1, 2, 3, 4}) | reduce([](int x) { return x; },
            [](int x, int y) { return x+y; });
    EXPECT_EQ(0+1+2+3+4, total);
}

TEST(Reduce, IdentitySumIntToFloat) {
    auto sv = MakeStream({0, 1, 2, 3, 4});
    auto total = sv.reduce([](int x) { return static_cast<float>(x); },
            [](float x, int y) { return x+static_cast<float>(y); });
    EXPECT_NEAR(0.f+1.f+2.f+3.f+4.f, total, 1e-5);
}

TEST(Reduce, PipeIdentitySumIntToFloat) {
    auto total = MakeStream({0, 1, 2, 3, 4}) |
        reduce([](int x) { return static_cast<float>(x); },
            [](float x, int y) { return x+static_cast<float>(y); });
    EXPECT_NEAR(0.f+1.f+2.f+3.f+4.f, total, 1e-5);
}

TEST(Reduce, SumSameType) {
    auto sv = MakeStream({0, 1, 2, 3, 4});
    auto total = sv.reduce([](int x, int y) { return x+y; });
    EXPECT_EQ(0+1+2+3+4, total);
}

TEST(Reduce, PipeSumSameType) {
    auto total = MakeStream({0, 1, 2, 3, 4}) |
        reduce([](int x, int y) { return x+y; });
    EXPECT_EQ(0+1+2+3+4, total);
}

TEST(Reduce, SumIntToFLoat) {
    auto &&vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    auto total = sv.reduce([](int x, int y) {
            return static_cast<float>(x+y); });
    EXPECT_NEAR(0.f+1.f+2.f+3.f+4.f, total, 1e-5);
}

TEST(Reduce, PipeSumIntToFLoat) {
    auto &&vec = std::vector<int>{0, 1, 2, 3, 4};
    auto total = MakeStream(vec) |
        reduce([](int x, int y) {
            return static_cast<float>(x+y); });
    EXPECT_NEAR(0.f+1.f+2.f+3.f+4.f, total, 1e-5);
}
