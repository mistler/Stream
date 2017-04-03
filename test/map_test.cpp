#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

// Helper functions
static int square_int(int i) { return i*i; }

static float half_int_as_float(int i) {
    return static_cast<float>(i)/2.f;
}

TEST(Map, SquareInt) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    int (*f)(int i) = square_int;
    sv = sv.map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(Map, HalfIntAsFloat) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    float (*f)(int i) = half_int_as_float;
    auto &sv_float = sv.map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_NEAR(static_cast<float>(vec[i])/2.f, sv_float.nth(i), 1e-5);
    }
}

// TODO: fix me: error that sv_float is different object than sv.
#if 0
TEST(Map, HalfIntAsFloat) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    float (*f)(int i) = half_int_as_float;
    auto sv_float = sv.map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_NEAR(static_cast<float>(vec[i])/2.f, sv_float.nth(i), 1e-5);
    }
}
#endif

TEST(Map, SquareIntLambdaPassByValue) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    auto tt = [](auto x) { return x*x; };
    sv = sv.map(tt);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

// TODO: enable me when forwarding works correctly
#if 0
TEST(Map, SquareIntLambdaTemporaryObject) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    sv = sv.map([](auto x) { return x*x; });
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}
#endif
