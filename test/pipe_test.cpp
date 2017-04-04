#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

using namespace stream;

// helper functions
static int square_int(int i) { return i*i; }

TEST(Pipe, MapSquareInt) {
    int (*f)(int i) = square_int;
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec) | map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(Pipe, Lambda11SquareInt) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec) | [](int x) { return x*x; };
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(Pipe, Lambda14SquareInt) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec) | [](auto x) { return x*x; };
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(Pipe, ToVector) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4, 5};
    auto s = MakeStream(vec);
    auto s_vec = s | to_vector();
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_vec[i]);
    }
}
