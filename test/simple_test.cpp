#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

#define UNUSED(x) (void)(x)

using namespace stream;

TEST(StreamTest, MakeStreamInitList) {
    auto s = MakeStream({1, 2, 3});
    UNUSED(s);
}

TEST(StreamTest, MakeStreamViaIterator) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    constexpr int arr_len = 3;
    double arr[arr_len]{};

    auto sv = MakeStream(vec.begin(), vec.end());
    auto sa = MakeStream(arr, arr+arr_len);
    UNUSED(sv);
    UNUSED(sa);
}

/*
TEST(StreamTest, SimpleTest) {
    auto s = MakeStream{1, 2, 3, 4, 5};
    s | map([](auto x) { return x * x; })
        | print_to(std::cout) << std::endl;
}

TEST(StreamTest, IteratorTest) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    for (int i = 0; i < vec.size(); ++i) {
        CHECK_EQ(s.nth(i), vec[i]);
    }
}
*/
