#include "gtest/gtest.h"

#include "src/stream.hpp"

#include <iostream>
#include <vector>

using namespace stream;

#define UNUSED(v) (void)(v)
#define PRINT(str) do { \
        std::cout << (str) << std::endl; \
    } while(false)

struct Perfect {
    Perfect() { PRINT("Perfect: constructor"); }
    ~Perfect() { PRINT("Perfect: destructor"); }

    Perfect(const Perfect &) { PRINT("Perfect: copy constructor"); }
    Perfect(Perfect &&) { PRINT("Perfect: move constructor"); }

    Perfect &operator=(const Perfect &) {
        PRINT("Perfect: copy assignment"); return *this;
    }
    Perfect &operator=(Perfect &&) {
        PRINT("Perfect: move assignment"); return *this;
    }
};

TEST(PerfectForwarding, MakeStreamIterator) {
    auto v = std::vector<Perfect> { Perfect() };
    auto s = MakeStream(v.begin(), v.end());
    UNUSED(s);
}

TEST(PerfectForwarding, MakeStreamInitList) {
    auto s = MakeStream({Perfect()});
    UNUSED(s);
}

TEST(PerfectForwarding, MakeStreamConstContainer) {
    const auto &cont = std::vector<Perfect> { Perfect() };
    auto s = MakeStream(cont);
    UNUSED(s);
}

TEST(PerfectForwarding, MakeStreamRvalueContainer) {
    auto &&cont = std::vector<Perfect> { Perfect() };
    auto s = MakeStream(cont);
    UNUSED(s);
}
