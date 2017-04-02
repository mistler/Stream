COPTS = [
    "-std=c++14",
    "-Werror",
]

GTEST_COPTS = COPTS + ["-Iexternal/gtest/include"]

cc_test(
    name = "test",
    srcs = ["simple_test.cpp"],
    copts = GTEST_COPTS,
    deps = [
            "@gtest//:main",
            "//src:stream"
           ],
    timeout = "short",
)
