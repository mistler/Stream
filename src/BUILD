COPTS = [
    "-std=c++14",
    "-Werror",
]

cc_library (
    name = "stream",
    hdrs = [
            "stream.hpp",
            "logger.hpp",
            "stream_operator.hpp",
            "type_helpers.hpp",
            "closure.hpp",
    ],
    srcs = ["stream_operator.cpp", "stream.cpp"],
    visibility = ["//test:__pkg__"],
    copts = COPTS,
)
