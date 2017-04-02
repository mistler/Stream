#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <sstream>
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
    double arr[arr_len]{0.0, 1.0, 2.0};

    auto sv = MakeStream(vec.begin(), vec.end());
    auto sa = MakeStream(arr, arr+arr_len);
    UNUSED(sv);
    UNUSED(sa);
}

TEST(StreamTest, MakeStreamFromContainer) {
    const auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    UNUSED(sv);
}

TEST(StreamTest, MakeStreamFromContainerEmpty) {
    const auto vec = std::vector<int>(0);
    auto sv = MakeStream(vec);
    UNUSED(sv);
}

TEST(StreamTest, MakeStreamFromContainerMove) {
    auto &&vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    UNUSED(sv);
}

#if 0
template<typename GenType>
struct SimpleGenerator {
    GenType operator()() { return GenType(); };
};

TEST(StreamTest, MakeStreamWithGenerator) {
    auto &&gen = SimpleGenerator<float>();
    auto s = MakeStream(gen);
    UNUSED(s);
}

TEST(StreamTest, SimpleTest) {
    auto s = MakeStream{1, 2, 3, 4, 5};
    s | map([](auto x) { return x * x; })
        | print_to(std::cout) << std::endl;
}
#endif

TEST(StreamTest, sum) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    int sum;
    for (const int &t : vec) {
        sum += t;
    }
    EXPECT_EQ(sum, s.sum().nth(0));
}

TEST(StreamTest, print_to) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    std::stringstream ss;
    s.print_to(ss);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        int actual, expected;
        expected = *it;
        ss >> actual;
        EXPECT_EQ(expected, actual);
    }
}

TEST(StreamTest, Nth) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s.nth(i));
    }
}

TEST(StreamTest, ToVector) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    std::vector<int> s_vec = s.to_vector();
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_vec[i]);
    }
}
