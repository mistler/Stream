#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <sstream>
#include <vector>

#define UNUSED(x) (void)(x)

using namespace stream;

// Helper functions
static int square_int(int i) { return i*i; }

static float half_int_as_float(int i) {
    return static_cast<float>(i)/2.f;
}

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

TEST(StreamTest, mapSquareInt) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    int (*f)(int i) = square_int;
    sv = sv.map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(StreamTest, mapHalfIntAsFloat) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    float (*f)(int i) = half_int_as_float;
    auto sv_float = sv.map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_NEAR(static_cast<float>(vec[i])/2.f, sv_float.nth(i), 1e-5);
    }
}

TEST(StreamTest, mapSquareIntLambda) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    sv = sv.map([](auto x) { return x*x; });
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(StreamTest, reduceWithIdentitySumSameType) {
    auto sv = MakeStream({0, 1, 2, 3, 4});
    auto total = sv.reduce([](int x) { return x; },
            [](int x, int y) { return x+y; });
    EXPECT_EQ(0+1+2+3+4, total);
}

TEST(StreamTest, reduceWithIdentitySumIntToFloat) {
    auto sv = MakeStream({0, 1, 2, 3, 4});
    auto total = sv.reduce([](int x) { return static_cast<float>(x); },
            [](float x, int y) { return x+static_cast<float>(y); });
    EXPECT_NEAR(0.f+1.f+2.f+3.f+4.f, total, 1e-5);
}

TEST(StreamTest, reduceSumSameType) {
    auto sv = MakeStream({0, 1, 2, 3, 4});
    auto total = sv.reduce([](int x, int y) { return x+y; });
    EXPECT_EQ(0+1+2+3+4, total);
}

// TODO: enable me after support of different return type is implemented!!!
#if 0
TEST(StreamTest, reduceSumIntToFLoat) {
    auto &&vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec);
    auto total = sv.reduce([](int x, int y) {
            return static_cast<float>(x+y); });
    EXPECT_NEAR(0.f+1.f+2.f+3.f+4.f, total, 1e-5);
}
#endif

TEST(StreamTest, filterEvenNumbers) {
    auto sv = MakeStream({0, 1, 2, 3, 4, 5});
    sv = sv.filter([](int x) { return (x%2) == 0; });
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(i*2, sv.nth(i));
    }
}

#if 0
TEST(StreamTest, pipeOperatorMapSquareInt) {
    int (*f)(int i) = square_int;
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec) | map(f);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(StreamTest, pipeOperatorLambda11SquareInt) {
    int (*f)(int i) = square_int;
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec) | [](int x) { return x*x; };
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}

TEST(StreamTest, pipeOperatorLambda14SquareInt) {
    int (*f)(int i) = square_int;
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto sv = MakeStream(vec) | [](auto x) { return x*x; };
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i]*vec[i], sv.nth(i));
    }
}
#endif

#if 0
TEST(StreamTest, pipeOperatorToVector) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4, 5};
    auto s = MakeStream(vec);
    auto s_vec = s | to_vector();
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], s_vec[i]);
    }
}
#endif

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
#endif

TEST(StreamTest, skip) {
    auto vec = std::vector<int>{0, 1, 2, 3, 4};
    auto s = MakeStream(vec.begin(), vec.end());
    constexpr int skip = 2;
    const int vlen = vec.size();
    s = s.skip(skip);
    for (int i = skip; i < vlen; i++) {
        EXPECT_EQ(vec[i], s.nth(i-skip));
    }
}

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

#if 0
TEST(StreamTest, SimpleTest) {
    auto s = MakeStream{1, 2, 3, 4, 5};
    s | map([](auto x) { return x * x; })
        | print_to(std::cout) << std::endl;
}
#endif

