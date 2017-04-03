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

// TODO: enable me
#if 0
template<typename GenType>
struct SimpleGenerator {
    GenType operator()() { return GenType(); };
};
TEST(MakeStream, MakeStreamWithGenerator) {
    auto &&gen = SimpleGenerator<float>();
    auto s = MakeStream(gen);
    UNUSED(s);
}
#endif
