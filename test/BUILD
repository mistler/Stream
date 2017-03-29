cc_test(
    name = "test",
    srcs = ["simple_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
            "@gtest//:main",
            "//src:stream"
           ],
    timeout = "short",
)
