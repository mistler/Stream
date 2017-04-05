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

TEST(Map, SquareIntTwoStreams) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    int (*f)(int i) = square_int;
    auto sv1 = sv.map(f);
    auto sv2 = sv.map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv1.nth(i));
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv2.nth(i));
    }
}

TEST(Map, PipeSquareInt) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    int (*f)(int i) = square_int;
    auto sv = MakeStream(vec) | map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(Map, HalfIntAsFloat) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    float (*f)(int i) = half_int_as_float;
    auto sv_float = sv.map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_NEAR(static_cast<float>(vec[i])/2.f, sv_float.nth(i), 1e-5);
    }
}

TEST(Map, SquareIntLambdaPassByValue) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    auto tt = [](auto x) { return x*x; };
    auto svv = sv.map(tt);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], svv.nth(i));
    }
}

TEST(Map, SquareIntLambdaTemporaryObject) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    sv = sv.map([](auto x) { return x*x; });
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}
