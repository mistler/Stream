#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <sstream>
#include <vector>

using namespace stream;

TEST(PrintTo, StringStream) {
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
