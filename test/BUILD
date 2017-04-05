COPTS = [
    "-std=c++14",
    "-Werror",
]

GTEST_COPTS = COPTS + ["-Iexternal/gtest/include"]

cc_test(
    name = "test",
    srcs = [
            "makestream_test.cpp",
            "nth_test.cpp",
            "skip_test.cpp",
            "printto_test.cpp",
            "tovector_test.cpp",
            "filter_test.cpp",
            "sum_test.cpp",
            "map_test.cpp",
            "reduce_test.cpp",
            "group_test.cpp",
            "pipe_test.cpp",
            "simple_test.cpp",
            "perfectforwarding_test.cpp",
    ],
    copts = GTEST_COPTS,
    deps = [
            "@gtest//:main",
            "//src:stream"
    ],
    timeout = "short",
)
