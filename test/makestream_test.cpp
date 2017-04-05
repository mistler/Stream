#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <vector>

#define UNUSED(x) (void)(x)

using namespace stream;

TEST(MakeStream, InitList) {
    auto s = MakeStream({1, 2, 3});
    UNUSED(s);
}

TEST(MakeStream, ViaIterator) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};

    constexpr int arr_len = 3;
    double arr[arr_len]{0.0, 1.0, 2.0};

    auto sv = MakeStream(vec.begin(), vec.end());
    auto sa = MakeStream(arr, arr+arr_len);
    UNUSED(sv);
    UNUSED(sa);
}

TEST(MakeStream, FromContainer) {
    const auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    UNUSED(sv);
}

TEST(MakeStream, FromContainerEmpty) {
    const auto vec = std::vector<int>(0);
    auto sv = MakeStream(vec);
    UNUSED(sv);
}

TEST(MakeStream, FromContainerMove) {
    auto &&vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    UNUSED(sv);
}

template<typename GenType>
struct SimpleGenerator {
    GenType operator()() { return GenType(); };
};
TEST(MakeStream, MakeStreamWithGenerator) {
    auto s1 = MakeStream(SimpleGenerator<float>());
    auto s2 = MakeStream([] () { return 3; });
    UNUSED(s1);
    UNUSED(s2);
}

TEST(MakeStream, MakeStreamVariadic) {
    auto sint = MakeStream();
    auto sint1 = MakeStream(3);
    auto sint2 = MakeStream(3, 4);
    auto sint3 = MakeStream(3, 4, 5);
    auto sint4 = MakeStream(3, 4, 5, 6);
    auto sfloat = MakeStream(3.f, 4.f, 5.f);

    UNUSED(sint);
    UNUSED(sint1);
    UNUSED(sint2);
    UNUSED(sint3);
    UNUSED(sint4);
    UNUSED(sfloat);
}
